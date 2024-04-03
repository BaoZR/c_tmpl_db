#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

typedef struct {
	char** names;         // 存储模板数据的数组
	unsigned char* data; // 存储模板名称的数组
	size_t capacity;     // 模板数据库的容量
	size_t tmpl_size;    // 单个模板的大小
	size_t num;          // 当前存储的模板数量
}TmplDB;

// 初始化模板数据库
TmplDB* init_empty_tmpl_db(size_t tmpl_size)
{
	int init_size = 10;
	TmplDB* db = calloc(1, sizeof(TmplDB));
	if (db == NULL) {
		return NULL;
	}
	unsigned char* buff = calloc(init_size, tmpl_size * sizeof(unsigned char));
	if (buff == NULL) {
		free(db);
		return NULL;
	}
	void* names = calloc(init_size, sizeof(char*));
	if (names == NULL) {
		free(buff);
		free(db);
		return NULL;
	}

	db->data      = buff;
	db->capacity  = init_size;
	db->names     = names;
	db->tmpl_size = tmpl_size;
	db->num       = 0;
	return db;
}

void free_tmpl_db(TmplDB* db)
{
	if (db == NULL)
		return;
	for (int i = 0; i < db->capacity; i++) {
		free(db->names[i]);
		db->names[i] = NULL;
	}
	free(db->names);
	db->names = NULL;
	free(db->data);
	db->data = NULL;
	free(db);
	db = NULL;
}

bool add_tmpl(TmplDB* db,unsigned char* copy_data,const char* name) {
	if (db == NULL || copy_data == NULL || name == NULL || strlen(name) > 32 ) {
		return false;
	}
	//如果空间小，先扩容
	if (db->num + 1 > db->capacity)
	{
		void* new_names = realloc(db->names, sizeof(char*) * (db->capacity * 2));
		if (new_names == NULL)
		{
			return false;
		}
		db->names = new_names;

		unsigned char* new_data = realloc(db->data, (db->tmpl_size) * sizeof(unsigned char) * (db->capacity *2));
		if (new_data == NULL) {
			return false;
		}
		db->data = new_data;

		db->capacity = db->capacity * 2;
	}

	unsigned char* name_ptr = calloc((strlen(name) + 1) ,sizeof(char));
	if (name_ptr == NULL) {
		return false;
	}
	memcpy(name_ptr, name, strlen(name));
	db->names[db->num] = name_ptr;
	memcpy(db->data + db->num * db->tmpl_size, copy_data, db->tmpl_size);
	db->num++;
	return true;
}




int main() {
	TmplDB *db = init_empty_tmpl_db(4);
	unsigned char copy_data[3] = { 'A','B' ,'C'};

	add_tmpl(db, copy_data, "aabb");

	assert(db->names[0][2] == 'b');
	assert(db->data[2] == 'C');

	add_tmpl(db, copy_data, "ddee");
	assert(db->names[1][2] == 'e');
	assert(db->data[6] == 'C');

	free_tmpl_db(db);
}