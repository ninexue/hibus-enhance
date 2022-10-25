#ifndef __HIBUS_BUSYBOX_FS__
#define __HIBUS_BUSYBOX_FS__

struct wildcard_list
{
    char *wildcard;
    struct wildcard_list *next;
};


#define	PROCEDURE_PARAM	hibus_conn* conn, const char* from_endpoint, \
            const char* to_method, const char* method_param, int *err_code

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
 * Subroutine:  fs_register
 * Function:    register procedure and event for busybox runner
 * Input:       hibus_context[I], Hibus context
 * Output:      None
 * Description:
 * Date:        2022/07/20
 * ModifyRecord:
 * *************************************************************************/
void fs_register(hibus_conn *hibus_context);


/****************************************************************************
 * Subroutine:  fs_revoke
 * Function:    revoke procedure and event for busybox runner
 * Input:       hibus_context[I], Hibus context
 * Output:      None
 * Description:
 * Date:        2022/07/20
 * ModifyRecord:
 * *************************************************************************/
void fs_revoke(hibus_conn *hibus_context);


/****************************************************************************
 * Subroutine:  changeDirectory
 * Function:    change current working directory.
 * Input:       Hibus register procedure input
 * Output:      Hibus register procedure output
 * Description:
 * Date:        2022/07/20
 * ModifyRecord:
 * *************************************************************************/
char * changeDirectory(PROCEDURE_PARAM);


/****************************************************************************
 * Subroutine:  listDirectory
 * Function:    list all files and directorys in an indicated directory.
 * Input:       Hibus register procedure input
 * Output:      Hibus register procedure output
 * Description:
 * Date:        2022/07/20
 * ModifyRecord:
 * *************************************************************************/
char * listDirectory(PROCEDURE_PARAM);


/****************************************************************************
 * Subroutine:  removeFileDirectory
 * Function:    remove file or directory.
 * Input:       Hibus register procedure input
 * Output:      Hibus register procedure output
 * Description:
 * Date:        2022/07/20
 * ModifyRecord:
 * *************************************************************************/
char * removeFileDirectory(PROCEDURE_PARAM);


/****************************************************************************
 * Subroutine:  removeEmptyDirectory
 * Function:    remove an emptry directory
 * Input:       Hibus register procedure input
 * Output:      Hibus register procedure output
 * Description:
 * Date:        2022/07/20
 * ModifyRecord:
 * *************************************************************************/
char * removeEmptyDirectory(PROCEDURE_PARAM);


/****************************************************************************
 * Subroutine:  makeDirectory
 * Function:    make directory in an indicated directory.
 * Input:       Hibus register procedure input
 * Output:      Hibus register procedure output
 * Description:
 * Date:        2022/07/20
 * ModifyRecord:
 * *************************************************************************/
char * makeDirectory(PROCEDURE_PARAM);


/****************************************************************************
 * Subroutine:  unlinkFile
 * Function:    remove a file in an indicated directory.
 * Input:       Hibus register procedure input
 * Output:      Hibus register procedure output
 * Description:
 * Date:        2022/07/20
 * ModifyRecord:
 * *************************************************************************/
char * unlinkFile(PROCEDURE_PARAM);


/****************************************************************************
 * Subroutine:  touchFile
 * Function:    touch a file in an indicated directory.
 * Input:       Hibus register procedure input
 * Output:      Hibus register procedure output
 * Description:
 * Date:        2022/07/20
 * ModifyRecord:
 * *************************************************************************/
char * touchFile(PROCEDURE_PARAM);


#ifdef __cplusplus
}
#endif

#endif  // __HIBUS_BUSYBOX_FS__
