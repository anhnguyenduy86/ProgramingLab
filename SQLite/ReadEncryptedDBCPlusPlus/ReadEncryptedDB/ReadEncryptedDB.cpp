// ReadEncryptedDB.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include "../CustomSQLite3StaticLibrary/custom_sqlite3.h"

int main(int argc, char* argv[])
{
	int ret;
	char pass[10] = "lock";

	sqlite3* dp = NULL;
	ret = sqlite3_open("test.db", &dp);

	__try {
		if (ret != SQLITE_OK) {
			printf("Error: %s\n", sqlite3_errmsg(dp));
			__leave;
		}

		ret = sqlite3_key(dp, pass, strlen(pass));
		if (ret != SQLITE_OK) {
			printf("Error: %s\n", sqlite3_errmsg(dp));
			__leave;
		}

		sqlite3_stmt *stm = NULL;

		ret = sqlite3_prepare(dp, "select TextCol from TEST",-1, &stm, NULL);
		if (ret == SQLITE_OK && stm) {
			__try {
				int colCount = sqlite3_column_count(stm);
				printf("NColumns=%d\n", colCount);
				for (int col = 0; col < colCount; ++col) {
					printf("%d : %s %s\n", col,
						sqlite3_column_name(stm, col),
						sqlite3_column_decltype(stm, col));
				}
				for (;;) {
					ret = sqlite3_step(stm);
					if (ret == SQLITE_ROW) {
						const void* val = sqlite3_column_text(stm, 0);
						printf("%s\n", val == NULL ? "NULL" : val);
						continue;
					}
					break;
				}
				if (ret == SQLITE_ERROR) {
					printf("failed in the loop: %s\n", sqlite3_errmsg(dp));
				}
			}
			__finally {
				sqlite3_finalize(stm);
			}
		}
		else {
			printf("prepare failed: %s\n", sqlite3_errmsg(dp));
			__leave;
		}
	}
	__finally {
		sqlite3_close(dp);
		printf("closed\n");
	}
	getchar();
	return 0;
}

