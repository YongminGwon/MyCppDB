#include "DataManager.h"
#include "Search.h"
#include "FileManager.h"
#include <iostream>
#include "UI.h"
int main() {

    DataManager& dm = DataManager::getInstance();
    dm.loadDeletedRecords();
    UI ui;
    ui.run();
    dm.saveDeletedRecords();
    return 0;
}
