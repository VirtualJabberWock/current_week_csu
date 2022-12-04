#include "ExtendedTypes.h"

//private
#define hash(s) HASHMAP_Private_hash(s)
//...

void __InitHashNode(HashNode* node, int hash, lc_string key, string_t value)
{
	node->next = 0;
	node->hash = hash;
	node->key = key;
	node->value = value;
	node->__notnull__ = 1;
}


void InitHashMap(HashMap* map)
{
	map->put = HASHMAP_Default_put;
	map->get = HASHMAP_Default_get;
	map->dispose = HASHMAP_Default_dispose;
	map->_nodes = initArray(MAP_BLOCK_SIZE, sizeof(HashNode));
	map->__notnull__ = 1;
	for (int i = 0; i < MAP_BLOCK_SIZE; i++) {
		map->_nodes[i].__notnull__ = 0;
	}
	map->size = 0;
}

HashMap* NewHashMap()
{
	HashMap* hmap = malloc(sizeof(HashMap));
	InitHashMap(hmap);
	return hmap;
}

HashMap* WrapDataBox(DataBox* box)
{
	if (box->kv.size != box->vv.size) return
		panic_e(HASHMAP_CLASSNAME, "WrapDataBox",
			"Invalid dataBox : keys count should equal values count");
	HashMap* hmap = NewHashMap();
	for (int i = 0; i < box->kv.size; i++) {
		hmap->put(hmap, box->kv.ptr[i], box->vv.ptr[i]); 
	}
}

DataBox* DissectHashMap(HashMap* map)
{
	DataBox* box = (DataBox*) malloc(sizeof(DataBox));
	if (box == 0) return panic("Memory :: error!");
	InitDataBox(box);
	for (int i = 0; i < MAP_BLOCK_SIZE; i++) {
		HashNode* node = &(map->_nodes[i]);
		if (node == 0) continue;
		do {
			if (node->__notnull__) {
				box->kv.put(&box->kv, node->key);
				box->vv.put(&box->vv, node->value);
			}
			node = node->next;
		} while (node != 0);
	}
	return box;
}

void InitDataBox(DataBox* dB)
{
	dB->display = DATABOX_Private_display;
	InitStringV(&(dB->kv));
	InitStringV(&(dB->vv));
}

void HASHMAP_Default_put(__HMSelf__, lc_string key, string_t value){

	if (self->__notnull__ == 0)
		panic_NPE(HASHMAP_Default_get, "<HashMap> self");

	HASHMAP_Private_checkKey(key);
	
	int h = hash(key);
	int index = h % MAP_BLOCK_SIZE;

	if(DEBUG_MODE) printf("%s -> %s[%d]<%d>\n", value, key, index, h);

	if (self->_nodes[index].__notnull__ == 1) {
		HashNode* node = self->_nodes + index;
		while (node->next != 0) {
			node = node->next;
		}
		HashNode* tail = initArray(1, sizeof(HashNode));
		__InitHashNode(tail, h, key, value);
		node->next = tail;
	}
	else {
		__InitHashNode(&(self->_nodes[index]), h, key, value);
	}
	self->size++;
}

string_t HASHMAP_Default_get(__HMSelf__, lc_string key, errno_t* status){

	if (self->__notnull__ == 0)
		panic_NPE(HASHMAP_Default_get, "<HashMap> self");

	HASHMAP_Private_checkKey(key);
	int h = hash(key);
	int index = h % MAP_BLOCK_SIZE;
	if (DEBUG_MODE) printf("out <- %s[%d]\n", key, index);

	if (self->_nodes[index].__notnull__ == 1) {
		HashNode* node = self->_nodes + index;
		while (node->next != 0) {
			if (node->hash == h) {
				if (node->next == 0) break;
				if(strcmp(node->key, key) == 0) break;
			}
			node = node->next;
		}
		if(status != NULL) *status = ERR_NO;
		return node->value;
	}
	else {
		if (status != NULL) *status = ERR_KEY_DONT_EXIST;
		return "null";
	}
}

void HASHMAP_Default_dispose(__HMSelf__){
	if (self->__notnull__ == 0)
		panic_NPE(HASHMAP_Default_get, "<HashMap> self");
	if (self->_nodes == 0) {
		return panic_NPE(HASHMAP_Default_get, "<HashNode*> self->_nodes");
	}
	for (int i = 0; i < MAX_MAP_KEY_SIZE; i++) {
		if (self->_nodes[i].__notnull__ == 0) continue;
		HashNode* tmp;
		HashNode* head = &(self->_nodes[i]);
		while (head != NULL)
		{
			tmp = head;
			head = head->next;
			if(tmp != 0) free(tmp);
		}
	}
	if (self->_nodes != 0) {
		free(self->_nodes);
	}
	self->__notnull__ = 0;
}

int HASHMAP_Private_hash(string_t s) {
	int z = 0;
	int i = 0;
	while (s[i] != '\0') {
		z += (i ^ s[i])<<(i%32);
		i++;
	}
	return z;
}

void HASHMAP_Private_checkKey(string_t key) {
	int len = SUS_getStringLength(key);
	if (len > MAX_MAP_KEY_SIZE) 
		panic_e(
			HASHMAP_CLASSNAME, "checkKey(key)",
			"Key size bigger than max size"
		);
	for (int i = 0; i < len; i++) {
		char c = key[i];
		if ((c >= '0') | (c <= '9')) continue;
		if (((c < '@') & (c > '[')) | ((c < '`') & (c > '{'))) {
			panic_e(
				HASHMAP_CLASSNAME, "checkKey(key)",
				"key must be contains only letter"
			);
		};
	}
}

void DATABOX_Private_display(DataBox* box)
{
	printf("\n");
	if (box->kv.size != box->vv.size) {
		printf("Invalid DataBox!");
		return;
	}
	printf("DataBox {\n");
	for (int i = 0; i < box->kv.size; i++) {
		printf("%s : %s", box->kv.ptr[i], box->vv.ptr[i]);
		printf("\n");
	}
	printf("}");
}
