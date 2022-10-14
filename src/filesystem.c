#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <net/if.h>
#include <sys/timerfd.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <linux/wireless.h>
#include <arpa/inet.h>
#include <errno.h>

#include <hibus.h>
#include <hibox/json.h>

#include "hibus_busybox.h"
#include "interface.h"
#include "helper.h"
#include "filesystem.h"

extern const char *op_errors[];

static struct busybox_procedure fs_procedure[] =
{
	{METHOD_HIBUS_BUSYBOX_LS, 		listDirectory},
	{METHOD_HIBUS_BUSYBOX_RM, 		removeFile},
	{METHOD_HIBUS_BUSYBOX_RMDIR, 	removeDirectory},
	{METHOD_HIBUS_BUSYBOX_MKDIR, 	makeDirectory},
	{METHOD_HIBUS_BUSYBOX_UNLINK, 	unlinkFile},
	{METHOD_HIBUS_BUSYBOX_TOUCH, 	touchFile}
};

static struct busybox_event fs_event [] = {};


char * listDirectory(hibus_conn* conn, const char* from_endpoint,
				const char* to_method, const char* method_param, int *err_code)
{
    char * ret_string = malloc(8192);
    int ret_code = ERROR_BUSYBOX_OK;
    hibus_json *jo = NULL;
    hibus_json *jo_tmp = NULL;
	uid_t euid;

    sprintf(ret_string, "{\"list\":[");

    // get procedure name
    if(strncasecmp(to_method, METHOD_HIBUS_BUSYBOX_LS,
								strlen(METHOD_HIBUS_BUSYBOX_LS)))
    {
        ret_code = ERROR_BUSYBOX_WRONG_PROCEDURE;
        goto failed;
    }

    jo = hibus_json_object_from_string(method_param, strlen(method_param), 2);
    if(jo == NULL)
    {
        ret_code = ERROR_BUSYBOX_WRONG_JSON;
        goto failed;
    }

    // get euid
    if(json_object_object_get_ex(jo, "euid", &jo_tmp) == 0)
    {
        ret_code = ERROR_BUSYBOX_WRONG_JSON;
        goto failed;
    }
   	euid = json_object_get_int(jo_tmp);
	change_euid(from_endpoint, euid);

	// get file name
    char dir_name[PATH_MAX];
    char filename[PATH_MAX];
    const char *string_filename = NULL;
    const char *filter = NULL;
    struct wildcard_list *wildcard = NULL;
    struct wildcard_list *temp_wildcard = NULL;
    char au[10] = {0};
    int i = 0;

    if ((argv == NULL) || (nr_args < 1)) {
        purc_set_error (PURC_ERROR_INVALID_VALUE);
        return PURC_VARIANT_INVALID;
    }

    if (!purc_variant_is_string (argv[0])) {
        purc_set_error (PURC_ERROR_WRONG_DATA_TYPE);
        return PURC_VARIANT_INVALID;
    }

    // get the file name
    string_filename = purc_variant_get_string_const (argv[0]);
    strcpy (dir_name, string_filename);

    if (access(dir_name, F_OK | R_OK) != 0) {
        purc_set_error (PURC_ERROR_BAD_SYSTEM_CALL);
        return PURC_VARIANT_INVALID;
    }

    // get the filter
    if ((nr_args > 1) && (argv[1] == NULL ||
            (!purc_variant_is_string (argv[1])))) {
        purc_set_error (PURC_ERROR_WRONG_DATA_TYPE);
        return PURC_VARIANT_INVALID;
    }
    if ((nr_args > 1) && (argv[1] != NULL))
        filter = purc_variant_get_string_const (argv[1]);

    // get filter array
    if (filter) {
        size_t length = 0;
        const char *head = pcdvobjs_get_next_option (filter, ";", &length);
        while (head) {
            if (wildcard == NULL) {
                wildcard = malloc (sizeof(struct wildcard_list));
                if (wildcard == NULL)
                    goto error;
                temp_wildcard = wildcard;
            }
            else {
                temp_wildcard->next = malloc (sizeof(struct wildcard_list));
                if (temp_wildcard->next == NULL)
                    goto error;
                temp_wildcard = temp_wildcard->next;
            }
            temp_wildcard->next = NULL;
            temp_wildcard->wildcard = malloc (length + 1);
            if (temp_wildcard->wildcard == NULL)
                goto error;
            strncpy(temp_wildcard->wildcard, head, length);
            *(temp_wildcard->wildcard + length) = 0x00;
            pcdvobjs_remove_space (temp_wildcard->wildcard);
            head = pcdvobjs_get_next_option (head + length + 1, ";", &length);
        }
    }

    // get the dirctory content
    DIR *dir = NULL;
    struct dirent *ptr = NULL;
    purc_variant_t obj_var = PURC_VARIANT_INVALID;
    struct stat file_stat;

    if ((dir = opendir (dir_name)) == NULL) {
        purc_set_error (PURC_ERROR_BAD_SYSTEM_CALL);
        goto error;
    }

    ret_var = purc_variant_make_array (0, PURC_VARIANT_INVALID);
    while ((ptr = readdir(dir)) != NULL)
    {
        if (strcmp (ptr->d_name,".") == 0 || strcmp(ptr->d_name, "..") == 0)
            continue;

        // use filter
        temp_wildcard = wildcard;
        while (temp_wildcard) {
            if (wildcard_cmp (ptr->d_name, temp_wildcard->wildcard))
                break;
            temp_wildcard = temp_wildcard->next;
        }
        if (wildcard && (temp_wildcard == NULL))
            continue;

        obj_var = purc_variant_make_object (0, PURC_VARIANT_INVALID,
                PURC_VARIANT_INVALID);

        strcpy (filename, dir_name);
        strcat (filename, "/");
        strcat (filename, ptr->d_name);

        if (stat(filename, &file_stat) < 0)
            continue;

        // name
        val = purc_variant_make_string (ptr->d_name, false);
        purc_variant_object_set_by_static_ckey (obj_var, "name", val);
        purc_variant_unref (val);

        // dev
        val = purc_variant_make_number (file_stat.st_dev);
        purc_variant_object_set_by_static_ckey (obj_var, "dev", val);
        purc_variant_unref (val);

        // inode
        val = purc_variant_make_number (ptr->d_ino);
        purc_variant_object_set_by_static_ckey (obj_var, "inode", val);
        purc_variant_unref (val);

        // type
        if (ptr->d_type == DT_BLK) {
            val = purc_variant_make_string ("b", false);
            purc_variant_object_set_by_static_ckey (obj_var, "type", val);
            purc_variant_unref (val);
        }
        else if(ptr->d_type == DT_CHR) {
            val = purc_variant_make_string ("c", false);
            purc_variant_object_set_by_static_ckey (obj_var, "type", val);
            purc_variant_unref (val);
        }
        else if(ptr->d_type == DT_DIR) {
            val = purc_variant_make_string ("d", false);
            purc_variant_object_set_by_static_ckey (obj_var, "type", val);
            purc_variant_unref (val);
        }
        else if(ptr->d_type == DT_FIFO) {
            val = purc_variant_make_string ("f", false);
            purc_variant_object_set_by_static_ckey (obj_var, "type", val);
            purc_variant_unref (val);
        }
        else if(ptr->d_type == DT_LNK) {
            val = purc_variant_make_string ("l", false);
            purc_variant_object_set_by_static_ckey (obj_var, "type", val);
            purc_variant_unref (val);
        }
        else if(ptr->d_type == DT_REG) {
            val = purc_variant_make_string ("r", false);
            purc_variant_object_set_by_static_ckey (obj_var, "type", val);
            purc_variant_unref (val);
        }
        else if(ptr->d_type == DT_SOCK) {
            val = purc_variant_make_string ("s", false);
            purc_variant_object_set_by_static_ckey (obj_var, "type", val);
            purc_variant_unref (val);
        }
        else if(ptr->d_type == DT_UNKNOWN) {
            val = purc_variant_make_string ("u", false);
            purc_variant_object_set_by_static_ckey (obj_var, "type", val);
            purc_variant_unref (val);
        }

        // mode
        val = purc_variant_make_byte_sequence (&(file_stat.st_mode),
                                                    sizeof(unsigned long));
        purc_variant_object_set_by_static_ckey (obj_var, "mode", val);
        purc_variant_unref (val);

        // mode_str
        for (i = 0; i < 3; i++) {
            if ((0x01 << (8 - 3 * i)) & file_stat.st_mode)
                au[i * 3 + 0] = 'r';
            else
                au[i * 3 + 0] = '-';
            if ((0x01 << (7 - 3 * i)) & file_stat.st_mode)
                au[i * 3 + 1] = 'w';
            else
                au[i * 3 + 1] = '-';
            if ((0x01 << (6 - 3 * i)) & file_stat.st_mode)
                au[i * 3 + 2] = 'x';
            else
                au[i * 3 + 2] = '-';
        }
        val = purc_variant_make_string (au, false);
        purc_variant_object_set_by_static_ckey (obj_var, "mode_str", val);
        purc_variant_unref (val);

        // nlink
        val = purc_variant_make_number (file_stat.st_nlink);
        purc_variant_object_set_by_static_ckey (obj_var, "nlink", val);
        purc_variant_unref (val);

        // uid
        val = purc_variant_make_number (file_stat.st_uid);
        purc_variant_object_set_by_static_ckey (obj_var, "uid", val);
        purc_variant_unref (val);

        // gid
        val = purc_variant_make_number (file_stat.st_gid);
        purc_variant_object_set_by_static_ckey (obj_var, "gid", val);
        purc_variant_unref (val);

        // rdev_major
        val = purc_variant_make_number (major(file_stat.st_dev));
        purc_variant_object_set_by_static_ckey (obj_var, "rdev_major", val);
        purc_variant_unref (val);

        // rdev_minor
        val = purc_variant_make_number (minor(file_stat.st_dev));
        purc_variant_object_set_by_static_ckey (obj_var, "rdev_minor", val);
        purc_variant_unref (val);

        // size
        val = purc_variant_make_number (file_stat.st_size);
        purc_variant_object_set_by_static_ckey (obj_var, "size", val);
        purc_variant_unref (val);

        // blksize
        val = purc_variant_make_number (file_stat.st_blksize);
        purc_variant_object_set_by_static_ckey (obj_var, "blksize", val);
        purc_variant_unref (val);

        // blocks
        val = purc_variant_make_number (file_stat.st_blocks);
        purc_variant_object_set_by_static_ckey (obj_var, "blocks", val);
        purc_variant_unref (val);

        // atime
        val = purc_variant_make_string (ctime(&file_stat.st_atime), false);
        purc_variant_object_set_by_static_ckey (obj_var, "atime", val);
        purc_variant_unref (val);

        // mtime
        val = purc_variant_make_string (ctime(&file_stat.st_mtime), false);
        purc_variant_object_set_by_static_ckey (obj_var, "mtime", val);
        purc_variant_unref (val);

        // ctime
        val = purc_variant_make_string (ctime(&file_stat.st_ctime), false);
        purc_variant_object_set_by_static_ckey (obj_var, "ctime", val);
        purc_variant_unref (val);

        purc_variant_array_append (ret_var, obj_var);
        purc_variant_unref (obj_var);
    }

    closedir(dir);

failed:
    if(jo)
        json_object_put (jo);

    sprintf(ret_string + strlen(ret_string), "],\"errCode\":%d, \"errMsg\":\"%s\"}", ret_code, op_errors[-1 * ret_code]);

	restore_euid();

    while(wildcard)
	{
        if(wildcard->wildcard)
            free(wildcard->wildcard);
        temp_wildcard = wildcard;
        wildcard = wildcard->next;
        free (temp_wildcard);
    }

    return ret_string;

}

char * removeFile(hibus_conn* conn, const char* from_endpoint,
				const char* to_method, const char* method_param, int *err_code)
{
    char * ret_string = malloc(128);
    int ret_code = ERROR_BUSYBOX_OK;
    hibus_json *jo = NULL;
    hibus_json *jo_tmp = NULL;
	uid_t euid;

    // get procedure name
    if(strncasecmp(to_method, METHOD_HIBUS_BUSYBOX_RM,
								strlen(METHOD_HIBUS_BUSYBOX_LS)))
    {
        ret_code = ERROR_BUSYBOX_WRONG_PROCEDURE;
        goto failed;
    }

    // analyze json
    jo = hibus_json_object_from_string(method_param, strlen(method_param), 2);
    if(jo == NULL)
    {
        ret_code = ERROR_BUSYBOX_WRONG_JSON;
        goto failed;
    }

    // get device name
    if(json_object_object_get_ex(jo, "euid", &jo_tmp) == 0)
    {
        ret_code = ERROR_BUSYBOX_WRONG_JSON;
        goto failed;
    }

   	euid = json_object_get_int(jo_tmp);
	change_euid(from_endpoint, euid);


failed:
    if(jo)
        json_object_put (jo);

    sprintf(ret_string, "{\"errCode\":%d, \"errMsg\":\"%s\"}", ret_code, op_errors[-1 * ret_code]);

	restore_euid();

    return ret_string;
}

char * removeDirectory(hibus_conn* conn, const char* from_endpoint,
				const char* to_method, const char* method_param, int *err_code)
{
    char * ret_string = malloc(128);
    int ret_code = ERROR_BUSYBOX_OK;
    hibus_json *jo = NULL;
    hibus_json *jo_tmp = NULL;
	uid_t euid;

    // get procedure name
    if(strncasecmp(to_method, METHOD_HIBUS_BUSYBOX_RMDIR,
								strlen(METHOD_HIBUS_BUSYBOX_LS)))
    {
        ret_code = ERROR_BUSYBOX_WRONG_PROCEDURE;
        goto failed;
    }

    // analyze json
    jo = hibus_json_object_from_string(method_param, strlen(method_param), 2);
    if(jo == NULL)
    {
        ret_code = ERROR_BUSYBOX_WRONG_JSON;
        goto failed;
    }

    // get device name
    if(json_object_object_get_ex(jo, "euid", &jo_tmp) == 0)
    {
        ret_code = ERROR_BUSYBOX_WRONG_JSON;
        goto failed;
    }

   	euid = json_object_get_int(jo_tmp);
	change_euid(from_endpoint, euid);



failed:
    if(jo)
        json_object_put (jo);

    sprintf(ret_string, "{\"errCode\":%d, \"errMsg\":\"%s\"}", ret_code, op_errors[-1 * ret_code]);

	restore_euid();

    return ret_string;
}

char * makeDirectory(hibus_conn* conn, const char* from_endpoint,
				const char* to_method, const char* method_param, int *err_code)
{
    char * ret_string = malloc(128);
    int ret_code = ERROR_BUSYBOX_OK;
    hibus_json *jo = NULL;
    hibus_json *jo_tmp = NULL;
	uid_t euid;

    // get procedure name
    if(strncasecmp(to_method, METHOD_HIBUS_BUSYBOX_MKDIR,
								strlen(METHOD_HIBUS_BUSYBOX_LS)))
    {
        ret_code = ERROR_BUSYBOX_WRONG_PROCEDURE;
        goto failed;
    }

    // analyze json
    jo = hibus_json_object_from_string(method_param, strlen(method_param), 2);
    if(jo == NULL)
    {
        ret_code = ERROR_BUSYBOX_WRONG_JSON;
        goto failed;
    }

    // get device name
    if(json_object_object_get_ex(jo, "euid", &jo_tmp) == 0)
    {
        ret_code = ERROR_BUSYBOX_WRONG_JSON;
        goto failed;
    }

   	euid = json_object_get_int(jo_tmp);
	change_euid(from_endpoint, euid);


failed:
    if(jo)
        json_object_put (jo);

    sprintf(ret_string, "{\"errCode\":%d, \"errMsg\":\"%s\"}", ret_code, op_errors[-1 * ret_code]);

	restore_euid();

    return ret_string;
}

char * unlinkFile(hibus_conn* conn, const char* from_endpoint,
				const char* to_method, const char* method_param, int *err_code)
{
    char * ret_string = malloc(128);
    int ret_code = ERROR_BUSYBOX_OK;
    hibus_json *jo = NULL;
    hibus_json *jo_tmp = NULL;
	uid_t euid;

    // get procedure name
    if(strncasecmp(to_method, METHOD_HIBUS_BUSYBOX_UNLINK,
								strlen(METHOD_HIBUS_BUSYBOX_LS)))
    {
        ret_code = ERROR_BUSYBOX_WRONG_PROCEDURE;
        goto failed;
    }

    // analyze json
    jo = hibus_json_object_from_string(method_param, strlen(method_param), 2);
    if(jo == NULL)
    {
        ret_code = ERROR_BUSYBOX_WRONG_JSON;
        goto failed;
    }

    // get device name
    if(json_object_object_get_ex(jo, "euid", &jo_tmp) == 0)
    {
        ret_code = ERROR_BUSYBOX_WRONG_JSON;
        goto failed;
    }

   	euid = json_object_get_int(jo_tmp);
	change_euid(from_endpoint, euid);



failed:
    if(jo)
        json_object_put (jo);

    sprintf(ret_string, "{\"errCode\":%d, \"errMsg\":\"%s\"}", ret_code, op_errors[-1 * ret_code]);

	restore_euid();

    return ret_string;
}


char * touchFile(hibus_conn* conn, const char* from_endpoint,
				const char* to_method, const char* method_param, int *err_code)
{
    char * ret_string = malloc(128);
    int ret_code = ERROR_BUSYBOX_OK;
    hibus_json *jo = NULL;
    hibus_json *jo_tmp = NULL;
	uid_t euid;

    // get procedure name
    if(strncasecmp(to_method, METHOD_HIBUS_BUSYBOX_TOUCH,
								strlen(METHOD_HIBUS_BUSYBOX_LS)))
    {
        ret_code = ERROR_BUSYBOX_WRONG_PROCEDURE;
        goto failed;
    }

    // analyze json
    jo = hibus_json_object_from_string(method_param, strlen(method_param), 2);
    if(jo == NULL)
    {
        ret_code = ERROR_BUSYBOX_WRONG_JSON;
        goto failed;
    }

    // get device name
    if(json_object_object_get_ex(jo, "euid", &jo_tmp) == 0)
    {
        ret_code = ERROR_BUSYBOX_WRONG_JSON;
        goto failed;
    }

   	euid = json_object_get_int(jo_tmp);
	change_euid(from_endpoint, euid);

failed:
    if(jo)
        json_object_put (jo);

    sprintf(ret_string, "{\"errCode\":%d, \"errMsg\":\"%s\"}", ret_code, op_errors[-1 * ret_code]);

	restore_euid();

    return ret_string;
}


void fs_register(hibus_conn *context)
{
	size_t i = 0;
	size_t size = sizeof(fs_procedure) / sizeof(struct busybox_procedure);
    int ret_code = 0;

	for(i = 0; i < size; i++)
	{
    	ret_code = hibus_register_procedure(context, fs_procedure[i].name,
										NULL, NULL, fs_procedure[i].handler);
	    if(ret_code)
    	{
        	fprintf(stderr, "Busybox--filesystem: Error for register procedure"
							"%s, %s.\n", fs_procedure[i].name,
							hibus_get_err_message(ret_code));
	        return;
    	}
	}

	size = sizeof(fs_event) / sizeof(struct busybox_event);
	for(i = 0; i < size; i++)
	{
    	ret_code = hibus_register_event(context, fs_event[i].name, NULL, NULL);
	    if(ret_code)
    	{
        	fprintf(stderr, "Busybox--filesystem: Error for register event"
							"%s, %s.\n",
							fs_event[i].name, hibus_get_err_message(ret_code));
	        return;
    	}
	}
}

void fs_revoke(hibus_conn *context)
{
	size_t i = 0;
	size_t size = sizeof(fs_procedure) / sizeof(struct busybox_procedure);

	for(i = 0; i < size; i++)
    	hibus_revoke_procedure(context, fs_procedure[i].name);

	size = sizeof(fs_event) / sizeof(struct busybox_event);
	for(i = 0; i < size; i++)
		hibus_revoke_event(context, fs_event[i].name);

}
