#ifndef __HIBUS_BUSYBOX_FS__
#define __HIBUS_BUSYBOX_FS__

#ifdef __cplusplus
extern "C" {
#endif

char * listDirectory(hibus_conn* conn, const char* from_endpoint, const char* to_method, const char* method_param, int *err_code);
char * removeFile(hibus_conn* conn, const char* from_endpoint, const char* to_method, const char* method_param, int *err_code);
char * removeDirectory(hibus_conn* conn, const char* from_endpoint, const char* to_method, const char* method_param, int *err_code);
char * makeDirectory(hibus_conn* conn, const char* from_endpoint, const char* to_method, const char* method_param, int *err_code);
char * unlinkFile(hibus_conn* conn, const char* from_endpoint, const char* to_method, const char* method_param, int *err_code);
char * touchFile(hibus_conn* conn, const char* from_endpoint, const char* to_method, const char* method_param, int *err_code);

void fs_register(hibus_conn *hibus_context);
void fs_revoke(hibus_conn *hibus_context);

#ifdef __cplusplus
}
#endif

#endif  // __HIBUS_BUSYBOX_FS__
