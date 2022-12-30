#include "StorageManager.h"

StorageManager::StorageManager(const std::string& storageName)
	: m_storage(CreateStorage(storageName))
{
	m_storage.sync_schema();
	m_storageManagerSingleton = this;
}
StorageManager::~StorageManager()
{
	if (m_storageManagerSingleton == nullptr) {
		return;
	}
	delete m_storageManagerSingleton;
}

StorageManager& StorageManager::GetInstance()
{
	if (m_storageManagerSingleton == nullptr) {
		m_storageManagerSingleton = new StorageManager;
	}
	return *m_storageManagerSingleton;
}
Storage& StorageManager::GetStorage()
{
	return m_storage;
}
