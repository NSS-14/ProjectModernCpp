#pragma once

#include <string>

#include "Storage.h"

class StorageManager
{
public:
	StorageManager(const StorageManager&) = delete;
	StorageManager(StorageManager&&) = delete;
	StorageManager& operator =(const StorageManager&) = delete;
	StorageManager& operator =(StorageManager&&) = delete;
	bool operator ==(const StorageManager&) = delete;
	bool operator ==(StorageManager&&) = delete;
	~StorageManager();

public:
	static StorageManager& GetInstance();
	Storage& GetStorage();

private:
	StorageManager(const std::string& storageName = "database.sqlite");

private:
	inline static StorageManager* m_storageManagerSingleton = nullptr;
	Storage m_storage;
};

