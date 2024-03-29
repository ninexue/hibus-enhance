#ifndef __HIBUS_BUSYBOX_FS__
#define __HIBUS_BUSYBOX_FS__

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
 * Subroutine:  fs_register
 * Function:    register file system procedure and event for busybox runner
 * Input:       hibus_context[I], Hibus context
 * Output:      None
 * Description:
 * Date:        2022/07/20
 * ModifyRecord:
 * *************************************************************************/
void fs_register(hibus_conn *hibus_context);


/****************************************************************************
 * Subroutine:  fs_revoke
 * Function:    revoke file system procedure and event for busybox runner
 * Input:       hibus_context[I], Hibus context
 * Output:      None
 * Description:
 * Date:        2022/07/20
 * ModifyRecord:
 * *************************************************************************/
void fs_revoke(hibus_conn *hibus_context);


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
