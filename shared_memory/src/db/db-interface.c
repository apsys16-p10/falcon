#include <stdlib.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <db.h>
#include "../include/db/db-interface.h"
#include "../include/util/debug.h"

const char* db_dir="./.db";

struct db_t{
    DB* bdb_ptr;
};

void mk_path(char* dest,const char* prefix,const char* db_name){
    memcpy(dest,prefix,strlen(prefix));
    dest[strlen(prefix)] = '/';
    memcpy(dest+strlen(prefix)+1,db_name,strlen(db_name));
    dest[strlen(prefix)+strlen(db_name)+1] = '\0';
    return;
}

db* initialize_db(const char* db_name, uint32_t flag){
    db* db_ptr = NULL;
    DB* b_db;
    DB_ENV* dbenv;
    int ret;
    char* full_path = NULL;
    if((ret = mkdir(db_dir,S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)) != 0){
        if(errno!=EEXIST){
            con_err_log("DB : Dir Creation Failed\n");
            goto db_init_return;
        }
    }
    full_path = (char*)malloc(strlen(db_dir) + strlen(db_name) + 2);
    mk_path(full_path, db_dir, db_name);
    if ((ret = db_env_create(&dbenv, 0)) != 0) {
        dbenv->err(dbenv, ret, "Environment Created: %s", db_dir);
        goto db_init_return;
    }
    if ((ret = dbenv->open(dbenv, db_dir, DB_CREATE|DB_INIT_CDB|DB_INIT_MPOOL|DB_THREAD, 0)) != 0) {
        //dbenv->err(dbenv, ret, "Environment Open: %s", db_dir);
        goto db_init_return;
    }
    /* Initialize the DB handle */
    if((ret = db_create(&b_db,dbenv,flag)) != 0){
        con_err_log("DB : %s.\n", db_strerror(ret));
        goto db_init_return;
    }

    if((ret = b_db->open(b_db, NULL, db_name, NULL, DB_BTREE, DB_THREAD|DB_CREATE,0)) != 0){
        //b_db->err(b_db,ret,"%s","test.db");
        goto db_init_return;
    }
    db_ptr = (db*)(malloc(sizeof(db)));
    db_ptr->bdb_ptr = b_db;

db_init_return:
    if(full_path != NULL){
        free(full_path);
    }
    if(db_ptr != NULL){
        ;
    }
    return db_ptr;
}

int retrieve_record(db* db_p, size_t key_size, void* key_data, size_t* data_size, void** data){
    int ret = 1;
    if(NULL == db_p || NULL == db_p->bdb_ptr){
        if(db_p == NULL){
          con_err_log("DB retrieve_record : db_p is null.\n");
        } else{
          con_err_log("DB retrieve_record : db_p->bdb_ptr is null.\n");
        }
        goto db_retrieve_return;
    }
    DB* b_db = db_p->bdb_ptr;
    DBT key, db_data;
    memset(&key, 0, sizeof(key));
    memset(&db_data, 0, sizeof(db_data));
    key.data = key_data;
    key.size = key_size;
    db_data.flags = DB_DBT_MALLOC;
    if((ret = b_db->get(b_db, NULL, &key, &db_data, 0)) == 0){
    }else{
        con_err_log("DB : %s.\n", db_strerror(ret));
        goto db_retrieve_return;
    }
    if(!db_data.size){
        goto db_retrieve_return;
    }
    *data = db_data.data;
    *data_size = db_data.size;
db_retrieve_return:
    return ret;
}

int store_record(db* db_p, size_t key_size, void* key_data, size_t data_size, void* data){
    int ret = 1;
    if((NULL == db_p)||(NULL == db_p->bdb_ptr)){
        if(db_p == NULL){
          con_err_log("DB store_record : db_p is null.\n");
        } else{
          con_err_log("DB store_recor : db_p->bdb_ptr is null.\n");
        }
        goto db_store_return;
    }
    DB* b_db = db_p->bdb_ptr;
    DBT key,db_data;
    memset(&key, 0, sizeof(key));
    memset(&db_data, 0, sizeof(db_data));
    key.data = key_data;
    key.size = key_size;
    db_data.data = data;
    db_data.size = data_size;
    if ((ret = b_db->put(b_db,NULL,&key,&db_data,DB_AUTO_COMMIT)) == 0){
    }
    else{
        con_err_log("DB : %s.\n", db_strerror(ret));
    }
db_store_return:
    return ret;
}
