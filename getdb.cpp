#include "getdb.h"
#include <iostream>
#include <sql.h>
#include <sqlext.h>
#include <sstream>

std::string fetchFromTibero() {
    SQLHENV hEnv;
    SQLHDBC hDbc;
    SQLHSTMT hStmt;
    SQLRETURN ret;
    SQLCHAR outConnStr[1024];
    SQLSMALLINT outConnStrLen;
    std::ostringstream jsonResult;

    // Initialize JSON output
    jsonResult << "{\"data\":[";

    // Allocate environment handle
    SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);
    SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);

    // Allocate connection handle
    SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc);

    // Connect to Tibero (Update DSN, User, and Password)
    ret = SQLDriverConnect(
        hDbc,
        NULL,
        (SQLCHAR*)"DSN=TRUS_RTDB_64;UID=CP_REMOTE;PWD=cpremote123prod;",
        SQL_NTS,
        outConnStr, sizeof(outConnStr), &outConnStrLen,
        SQL_DRIVER_COMPLETE
    );

    if (SQL_SUCCEEDED(ret)) {
        std::cout << "Connected to Tibero successfully!" << std::endl;

        // Allocate statement handle
        SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

        // Execute SQL Query
        ret = SQLExecDirect(hStmt, (SQLCHAR*)"select accountcode, accountname, deposit, maxcreditlimit from msaccount where accountcode in ('JP0164L','JE1042L');", SQL_NTS);
        if (SQL_SUCCEEDED(ret)) {
            SQLCHAR accountcode[100];
			SQLCHAR accountname[100];
			SQLCHAR deposit[20];
			SQLCHAR maxcreditlimit[20];

            bool first = true;

            while (SQLFetch(hStmt) == SQL_SUCCESS) {
                if (!first) jsonResult << ",";
                first = false;

                SQLGetData(hStmt, 1, SQL_C_CHAR, accountcode,    sizeof(accountcode), NULL);
                SQLGetData(hStmt, 2, SQL_C_CHAR, accountname,    sizeof(accountname), NULL);
                SQLGetData(hStmt, 3, SQL_C_CHAR, deposit,        sizeof(deposit), NULL);
				SQLGetData(hStmt, 4, SQL_C_CHAR, maxcreditlimit, sizeof(maxcreditlimit), NULL);
                
                //jsonResult << "\"accountcode\":\"" << accountcode << "\"\n\"accountname\":\"" << accountname << "\"\n\"deposit\":\"" << deposit << "\"\n\"maxcreditlimit\":\"" << maxcreditlimit << "\"";
                jsonResult << "{\n"
                           << "    \"accountcode\": \"" << accountcode << "\",\n"
                           << "    \"accountname\": \"" << accountname << "\",\n"
                           << "    \"deposit\": \"" << deposit << "\",\n"
                           << "    \"maxcreditlimit\": \"" << maxcreditlimit << "\"\n"
                           << "}";
            }
        }
        else {
            std::cerr << "Failed to execute query!" << std::endl;
            return "{\"error\":\"Failed to execute query on Tibero\"}";
        }

        jsonResult << "]}";  // Close JSON output

        // Cleanup
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        SQLDisconnect(hDbc);
    }
    else {
        // Capture SQL error message
        SQLCHAR sqlState[6], message[256];
        SQLINTEGER nativeError;
        SQLSMALLINT messageLength;

        SQLGetDiagRec(SQL_HANDLE_DBC, hDbc, 1, sqlState, &nativeError, message, sizeof(message), &messageLength);
        std::cerr << "Connection Failed! SQL State: " << sqlState << " - " << message << std::endl;

        jsonResult << "{\"error\":\"Failed to connect to Tibero. Check credentials and DSN.\"}";
    }

    SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
    SQLFreeHandle(SQL_HANDLE_ENV, hEnv);

    return jsonResult.str();
}


std::string fetchFromTibero2()
{
     SQLHENV hEnv;
     SQLHDBC hDbc;
     SQLHSTMT hStmt;
     SQLRETURN ret;
     SQLCHAR outConnStr[1024];
     SQLSMALLINT outConnStrLen;
     std::ostringstream jsonResult;
     
     jsonResult << "{\"data\":[";
     SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);
     SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
     SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc);
     
     ret = SQLDriverConnect(hDbc,NULL,(SQLCHAR*)"DSN=SYS_AODB_64;UID=sys;PWD=tibero;",SQL_NTS, outConnStr, sizeof(outConnStr), &outConnStrLen,SQL_DRIVER_COMPLETE);
    
	 if (SQL_SUCCEEDED(ret)) {
		 std::cout << "Connected to Tibero2 successfully!" << std::endl;
		 SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
		 ret = SQLExecDirect(hStmt, (SQLCHAR*)"SELECT df.tablespace_name AS TableSpaceName, ROUND(SUM(df.bytes), 2) AS DiskSize, ROUND((SUM(df.bytes) - COALESCE(fs.free_MB, 0)) , 2) AS DiskUsage, ROUND(COALESCE(fs.free_MB, 0), 2) AS DiskFree, ROUND((COALESCE(fs.free_MB, 0) / SUM(df.bytes)) * 100, 2) AS Percentage FROM dba_data_files df LEFT JOIN ( SELECT tablespace_name, SUM(bytes) AS free_MB FROM dba_free_space WHERE tablespace_name IN (SELECT DISTINCT tablespace_name FROM dba_data_files) GROUP BY tablespace_name ) fs ON df.tablespace_name = fs.tablespace_name GROUP BY df.tablespace_name, fs.free_MB ORDER BY df.tablespace_name asc;", SQL_NTS);
		 if (SQL_SUCCEEDED(ret)) {
			 SQLCHAR TableSpaceName[100];
			 SQLCHAR DiskSize[40];
			 SQLCHAR DiskUsage[40];
			 SQLCHAR DiskFree[40];
			 SQLCHAR Percentage[40];
			 bool first = true;
			 while (SQLFetch(hStmt) == SQL_SUCCESS) {
				 if (!first) jsonResult << ",";
				 first = false;
                 SQLGetData(hStmt, 1, SQL_C_CHAR, TableSpaceName, sizeof(TableSpaceName), NULL);
                 SQLGetData(hStmt, 2, SQL_C_CHAR, DiskSize, sizeof(DiskSize), NULL);
                 SQLGetData(hStmt, 3, SQL_C_CHAR, DiskUsage, sizeof(DiskUsage), NULL);
                 SQLGetData(hStmt, 4, SQL_C_CHAR, DiskFree, sizeof(DiskFree), NULL);
                 SQLGetData(hStmt, 5, SQL_C_CHAR, Percentage, sizeof(Percentage), NULL);
				 jsonResult << "{\n"
					 << "    \"TableSpaceName\": \"" << TableSpaceName << "\",\n"
					 << "    \"DiskSize\": \"" << DiskSize << "\",\n"
					 << "    \"DiskUsage\": \"" << DiskUsage << "\",\n"
					 << "    \"DiskFree\": \"" << DiskFree << "\",\n"
					 << "    \"Percentage\": \"" << Percentage << "\"\n"
					 << "}";
			 }
		 }
		 else {
			 std::cerr << "Failed to execute query!" << std::endl;
			 return "{\"error\":\"Failed to execute query on Tibero\"}";
		 }
		 jsonResult << "]}";
		 SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
		 SQLDisconnect(hDbc);
	 }
	 else 
     {
         SQLCHAR sqlState[6], message[256];
         SQLINTEGER nativeError;
         SQLSMALLINT messageLength;
         SQLGetDiagRec(SQL_HANDLE_DBC, hDbc, 1, sqlState, &nativeError, message, sizeof(message), &messageLength);
         std::cerr << "Connection Failed! SQL State: " << sqlState << " - " << message << std::endl;
         jsonResult << "{\"error\":\"Failed to connect to Tibero. Check credentials ";
     }

     SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
     SQLFreeHandle(SQL_HANDLE_ENV, hEnv);

     return jsonResult.str();
}