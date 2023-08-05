#include "DB_Queries_DML.h"


const int silent_level{1};
const int verify_affected_rows_sl{1};    // sl - silent level
const int verify_param_count_sl{1};      // sl - silent level
const int verify_field_count_sl{1};      // sl - silent level
const int make_bind_params_sl{1};        // sl - silent level
const int make_bind_results_sl{1};       // sl - silent level
const int mysql_stmt_free_result_sl{1};  // sl - silent level
const int close_stmt_sl{1};              // sl - silent level
const int execute_stmt_sl{1};            // sl - silent level
const int affected_rows_sl{1};           // sl - silent level

  /* return the total affected rows */
  affected_rows_t affected_rows(MYSQL_STMT* stmt, std::string headline) {
    affected_rows_t res = mysql_stmt_affected_rows(stmt);
    if (silent_level >= affected_rows_sl) {
      std::cout << headline << " affected rows: " << res << std::endl;
      if (res == -1) {
        std::cout << headline << " mysql_stmt_affected_rows() failed "
                  << std::endl;
      }
    }
    return res;
  }

  /* verify the total affected rows */
  no_errors verify_affected_rows(MYSQL_STMT* stmt, size_t exp_count,
                                 std::string headline) {
    uint64_t res = mysql_stmt_affected_rows(stmt);
    if (res == -1) {
      std::cout << headline << " mysql_stmt_affected_rows() failed "
                << std::endl;
      return false;
    }
    if (res != exp_count) {
      if (silent_level >= verify_affected_rows_sl) {
        std::cout << headline << " total affected rows: " << res
                  << " expected: " << exp_count << std::endl;
      }
      return false;
    }
    return true;
  };

  /* verify the total fetched rows */
  no_errors verify_fetched_rows(size_t count, size_t exp_count,
                                std::string headline) {
    if (count != exp_count) {
      std::cout << headline << " MySQL failed to return all rows " << std::endl;
      std::cout << " returned: " << count << " expected: " << exp_count
                << std::endl;
      return false;
    }
    return true;
  };

  /* verify the parameters count */
  no_errors verify_param_count(MYSQL_STMT* stmt, size_t exp_count,
                               std::string headline) {
    size_t param_count = mysql_stmt_param_count(stmt);
    if (param_count != exp_count) {
      if (silent_level >= verify_param_count_sl) {
        std::cout << headline << " total parameters in stmt: " << param_count
                  << " expected: " << exp_count << std::endl;
      }
      return false;
    }
    return true;
  };

  /* verify fields count */
  no_errors verify_field_count(MYSQL_RES* result, uint exp_count,
                               std::string headline) {
    uint field_count = mysql_num_fields(result);
    if (field_count != exp_count) {
      if (silent_level >= verify_field_count_sl) {
        std::cout << headline << " total fields in res: " << field_count
                  << " expected: " << exp_count << std::endl;
      }
      return false;
    }
    return true;
  };

  // Step 1 - Create a prepared statement handler, prepare the statement on the
  // server
  MYSQL_STMT* prepare_stmt(MYSQL* mysql_arg, const std::string& query,
                           const std::string& headline) {
    MYSQL_STMT* stmt = mysql_stmt_init(mysql_arg);
    if (stmt &&
        mysql_stmt_prepare(stmt, query.c_str(), (ulong)query.length())) {
      mysql_stmt_close(stmt);
      std::cout << headline << " mysql_stmt_prepare() failed " << std::endl;
      std::cout << mysql_stmt_error(stmt) << std::endl;
      return nullptr;
    }
    return stmt;
  };

  // Step 2 - bind input data for the parameter markers (on the server side???)
  no_errors bind_parameters(MYSQL_STMT* stmt, MYSQL_BIND* bnd_params,
                            std::string headline) {
    if (mysql_stmt_bind_param(stmt, bnd_params)) {
      if (silent_level >= make_bind_params_sl) {
        std::cout << headline
                  << " mysql_stmt_bind_param() failed: " << std::endl;
        std::cout << headline << mysql_stmt_error(stmt) << std::endl;
      }
      return false;
    }
    return true;
  };

  // Step 3 - executes the prepared query associated with the statement handler
  no_errors execute_stmt(MYSQL_STMT* stmt, std::string headline) {
    int res = mysql_stmt_execute(stmt);
#if _DEBUG
    std::cout << std::endl;
    std::cout << headline << " execute statement result: " << res;
    std::cout << std::endl;
#endif

    if (bool(res)) {
      if (silent_level >= execute_stmt_sl) {
        std::cout << headline << " mysql_stmt_execute() failed: " << std::endl;
        std::cout << headline << mysql_stmt_error(stmt) << std::endl;
      }
      return false;
    }
    return true;
  };

  // Step ~7
  no_errors free_result_stmt(MYSQL_STMT* stmt, std::string headline) {
    if (mysql_stmt_free_result(stmt)) {
      if (silent_level >= mysql_stmt_free_result_sl) {
        std::cout << headline
                  << " mysql_stmt_free_result() failed: " << std::endl;
        std::cout << headline << mysql_stmt_error(stmt) << std::endl;
      }
      return false;
    }
#if _DEBUG
    std::cout << headline << "Freed " << std::endl;
#endif
    return true;
  };

  // Step 8 - mysql_stmt_close() - close the statement handler
  // Step 9 - free metadata result
  no_errors close_stmt(MYSQL* mysql, MYSQL_STMT* stmt,
                       MYSQL_RES* result_metadata, std::string headline) {
    if (mysql_stmt_close(stmt)) {
      if (silent_level >= close_stmt_sl) {
        std::cout << headline
                  << " ailed while closing the statement: " << std::endl;
        std::cout << headline << mysql_error(mysql) << std::endl;
      }
      return false;
      if (result_metadata) mysql_free_result(result_metadata);
      // result_metadata = nullptr; //???
    }
    return true;
  };

  // Steps 1 - 4 (without Step 3)
  template <class T>
  no_errors prepare(T& arg_struct, MYSQL* mysql) {
    auto& query = arg_struct.Query_struct;
    auto& result = arg_struct.Result_struct;
    no_errors no_errors{true};

    // Step 1 - initialize and prepare statement
    if (!(arg_struct.stmt =
              prepare_stmt(mysql, arg_struct.query, arg_struct.headline)))
      return 0;

    // Step 2 - parameter count verifying and binding
    no_errors &= verify_param_count(arg_struct.stmt, query.param_count,
                                    arg_struct.headline);
    no_errors &=
        bind_parameters(arg_struct.stmt, query.bind, arg_struct.headline);

    // Step 3 - no execution, it will be later

    // Step 4.1 - result metadata getting and verifying
    if (result.param_count) {
      arg_struct.result_metadata = mysql_stmt_result_metadata(arg_struct.stmt);
      no_errors &= verify_field_count(arg_struct.result_metadata,
                                      result.param_count, arg_struct.headline);
    }
    if (!no_errors) return false;
    return true;
  };

  // Steps 3 - 5 (without Step 4)
  template <class T>
  // Step 3 - executes the prepared query associated with the statement handler
  no_errors execute(T& arg_struct) {
    auto& result = arg_struct.Result_struct;
    no_errors no_errors{true};

    no_errors &= execute_stmt(arg_struct.stmt, arg_struct.headline);
    if (!no_errors) return false;

#if _DEBUG
    if (arg_struct.result_metadata != nullptr) {
      std::cout << arg_struct.headline << "Result metadata - ";
      MYSQL_FIELD* stmt_field_list =
          mysql_fetch_fields(arg_struct.result_metadata);
      size_t stmt_field_count = mysql_num_fields(arg_struct.result_metadata);
      for (size_t i = 0; i < stmt_field_count; i++) {
        std::cout << "| field " << i
                  << ". field name: " << stmt_field_list[i].name << ";"
                  << " field type: " << stmt_field_list[i].type << " |";
      }
      std::cout << std::endl;
    }
#endif
    // Step 5 - bind otput data for the results markers (on the server side???)
    /* Bind the result */
    if (bool(arg_struct.stmt) && bool(result.bind)) {
      bool res = mysql_stmt_bind_result(arg_struct.stmt, result.bind);
#if _DEBUG
      std::cout << arg_struct.headline << "Binded";
      std::cout << std::endl;
#endif
    };

    if (!no_errors) return false;

    return true;
  };

  // Steps 8, 9
  template <class T>
  no_errors close(T& arg_struct, MYSQL* mysql) {
    return close_stmt(mysql, arg_struct.stmt, arg_struct.result_metadata,
                      arg_struct.headline);
  };