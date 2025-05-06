#include <iostream>
#include <windows.h>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <cstdlib>
#include <ctime>
using namespace std;


void Story();
void mainMenu();
void newGame();
void loadGame();
void quitGame();
void saveGame();
void shopMap();
void explore();
void Combact();
void Moving();
void Hud();
void CreatingMonster();
void CombactHud();
void npcInteraction();
void questSystem();
void CreateCharacter();
void pauseMenu();
void showMap();
void initializeMap();
void viewLocationInfo();
void checkMapEncounter();
void findRandomItem();
void displayInventory();
void sellItems();
void useItemInCombat();

// Global variables
bool Intro = true, SecondStory = false;
std::string name = " ", race = " ", sex = " ";
int level = 0, xp = 0, health = 0, maxhealth = 0, totalhealth = 0, tolevel = 0, heal = 0;
std::string monstername[4] = {"Goblin", "Dwarf", "Orge", "Witch"};
std::string currentMonster = " ";
int monsterHp = 0, monsterxp = 0, monsterlevel = 0;
int counter = 0;
bool inTown = false;

// Item class definition
class Item {
public:
    string name;
    string type;
    int value;
    int quantity;

    Item(string n = "", string t = "", int v = 0, int q = 1) : 
        name(n), type(t), value(v), quantity(q) {}

    void display() const {
        cout << name << " (" << type << ")";
        if (type == "potion") cout << " - Restores " << value << " HP";
        else cout << " - +" << value << (type == "weapon" ? " damage" : " defense");
        if (quantity > 1) cout << " x" << quantity;
        cout << endl;
    }
};

const int Map_Size = 10; 
char gameMap[Map_Size][Map_Size];
int playerX = 5;
int playerY = 5;
vector<Item> inventory;
int gold = 100;

    int main(){
        srand(time(0));
        mainMenu();
        CreateCharacter();
        Intro = true;
        Story();
        Hud();
        Moving();
        return 0;
    }

    void findRandomItem() {
        string itemNames[] = {"Health Potion", "Mana Potion", "Rusty Sword", "Leather Armor"};
        string itemTypes[] = {"potion", "potion", "weapon", "armor"};
        int itemValues[] = {20, 15, 5, 10};
        
        int itemIndex = rand() % 4;
        Item foundItem(itemNames[itemIndex], itemTypes[itemIndex], itemValues[itemIndex]);
        
        cout << "You found a " << itemNames[itemIndex] << "!\n";
        inventory.push_back(foundItem);
    }
    
    void displayInventory() {
        system("cls");
        cout << "==== INVENTORY ====\n";
        cout << "Gold: " << gold << "\n\n";
        
        if (inventory.empty()) {
            cout << "Your inventory is empty.\n";
        } else {
            for (size_t i = 0; i < inventory.size(); i++) {
                cout << i+1 << ". ";
                inventory[i].display();
            }
        }
        
        cout << "\nPress any key to return...";
    }
    
    void sellItems() {
        if (inventory.empty()) {
            cout << "You have nothing to sell!\n";
            return;
        }
        
        displayInventory();
        cout << "\nEnter item number to sell (0 to cancel): ";
        int choice;
        cin >> choice;
        
        if (choice > 0 && choice <= inventory.size()) {
            Item item = inventory[choice-1];
            int sellPrice = item.value * 0.75; 
            gold += sellPrice;
            inventory.erase(inventory.begin() + choice - 1);
            cout << "Sold " << item.name << " for " << sellPrice << " gold.\n";
        }
    }
    void CreateCharacter() {
        system("cls");
        cout << "===== CHARACTER CREATION =====" << endl;
        cout << "Enter your character's name: ";
        cin >> name;
        
        cout << "Enter your character's race (Human/Elf/Dwarf/Orc): ";
        cin >> race;
        
        cout << "Enter your character's sex (Male/Female/Other): ";
        cin >> sex;
        
        int classChoice;
        cout << "\nChoose your class:\n";
        cout << "1. Warrior - High strength and defense\n";
        cout << "2. Mage - Powerful magic abilities\n";
        cout << "3. Rogue - Quick and agile\n";
        cout << "Enter your choice (1-3): ";
        cin >> classChoice;
        
        level = 1;
        xp = 0;
        tolevel = 76;
        
        switch(classChoice) {
            case 1: 
                totalhealth = 120;
                maxhealth = totalhealth;
                break;
            case 2: 
                totalhealth = 80;
                maxhealth = totalhealth;
                break;
            case 3: 
                totalhealth = 100;
                maxhealth = totalhealth;
                break;
            default:
                cout << "Invalid choice, defaulting to Warrior.\n";
                totalhealth = 120;
                maxhealth = totalhealth;
        }
        
        system("cls");
        cout << "===== CHARACTER SUMMARY =====" << endl;
        cout << "Name: " << name << endl;
        cout << "Race: " << race << endl;
        cout << "Sex: " << sex << endl;
        cout << "Class: " << 
            (classChoice == 1 ? "Warrior" : 
             classChoice == 2 ? "Mage" : "Rogue") << endl;
        cout << "Health: " << totalhealth << "/" << maxhealth << endl;
        cout << "Level: " << level << endl;
        cout << "============================" << endl;
        
        cout << "\nPress any key to begin your adventure...";
        system("pause > nul");
    }
    void useItemInCombat() {
        system("cls");
        cout << "==== ITEMS ====\n";
        
        bool hasUsableItems = false;
        for (size_t i = 0; i < inventory.size(); i++) {
            if (inventory[i].type == "potion") {
                cout << i+1 << ". ";
                inventory[i].display();
                hasUsableItems = true;
            }
        }
        
        if (!hasUsableItems) {
            cout << "You have no usable items!\n";
            system("pause");
            return;
        }
        
        cout << "\nSelect item to use (0 to cancel): ";
        int choice;
        cin >> choice;
        
        if (choice > 0 && choice <= inventory.size()) {
            if (inventory[choice-1].type == "potion") {
                totalhealth += inventory[choice-1].value;
                if (totalhealth > maxhealth) totalhealth = maxhealth;
                cout << "Used " << inventory[choice-1].name << ". Healed " 
                     << inventory[choice-1].value << " HP.\n";
                inventory.erase(inventory.begin() + choice - 1);
            } else {
                cout << "You can't use that in combat!\n";
            }
        }
        system("pause");
    }
    void pauseMenu() {
        int choice;
        do {
            system("cls");
            std::cout << "===== PAUSE MENU =====" << std::endl;
            std::cout << "1. Resume Game" << std::endl;
            std::cout << "2. Save Game" << std::endl;
            std::cout << "3. Return to Main Menu" << std::endl;
            std::cout << "4. Quit Game" << std::endl;
            std::cout << "======================" << std::endl;
            std::cout << "Enter your choice: ";
            std::cin >> choice;
    
            switch(choice) {
                case 1:
                    system("cls");
                    return;
                case 2:
                    saveGame();
                    break;
                case 3:
                    mainMenu();
                    return;  
                case 4:
                    quitGame();
                    return;  
                default:
                    std::cout << "Invalid choice. Please try again." << std::endl;
                    Sleep(1000);
            }
        } while (true);
    }

    void Hud() {
        Sleep(500);
        system("cls");
        cout << "Name: " << name << "\nHealth: " << totalhealth 
                  << "\nRace: " << race << "\nSex: " << sex << "\nLevel: " << level;
        cout << "\nXP: " << xp << "\nXP to Level: " << tolevel 
                  << "\nGold: " << gold << endl; 
    }
    void CombactHud(){
        Sleep(500);
        system("cls");
        cout<< "Name: "<< name<< "\nHealth: "<< totalhealth<< "\nLevel: "<< level;
        cout<<"\nMonster Name: "<< currentMonster<< "\nMonsters Health: "<< monsterHp<< "\nMonster Level: "<< monsterlevel<< std::endl;
        
        int( playattack);
        int playerDamage = 8 * level /2;
        int monsterAttack = 6 * monsterlevel /2;
    }
    void Combact() {
        CombactHud();
        int playerAttack;
        int playerDamage = 8 * level;
        int monsterattack = 6 * monsterlevel / 2;
    
        if (totalhealth >= 1 && monsterHp >= 1) {
            cout << endl;
            cout << "1. Attack" << endl; 
            cout << "2. Block!" << endl;
            cout << "3. Use Item" << endl; 
            cout << "4. Run!" << endl;
            cout << "5. Pause Menu" << endl;
            cout << endl << endl;
            cin >> playerAttack;
            if (playerAttack == 4) {
                cout<<"Using Skill Move"<<endl;
                useItemInCombat();
                Combact();
            }

        if(playerAttack==1){
            std::cout<< "Attaking... You did "<< playerDamage<< " to the " << currentMonster << std::endl;
            monsterHp = monsterHp - playerDamage;
            Sleep(1000);
            CombactHud();
            if(monsterHp>= 1){
                std::cout<<"\n\n";
                std::cout<< "Monster is Attacking..."<<std::endl;
                totalhealth = totalhealth - monsterattack;
                std::cout<< "You suffer "<< monsterattack<< " hp "<< totalhealth<< std::endl;
                if(totalhealth <=0){
                    totalhealth= 0;
                }
            }
            else if(monsterHp <=0){
                monsterHp =0;
            }
            Sleep(1000);
            CombactHud();
            Combact();
        }
        else if(playerAttack == 2){
            std::cout<< "Blocking "<< std::endl;
            int i =rand()%100 +1;
            if(i>=50){
                std::cout<< "You blocked succesfully!"<< std::endl;
                heal = level +10 /2;
                std::cout<< "You healed "<< heal << "to your total health"<< std::endl;
                totalhealth +=heal;
                Sleep(2000);
                CombactHud();
                Combact();
            }
            else {
                cout<< "You failed to Block"<< endl;
                totalhealth = totalhealth - monsterattack;
                cout<< "You suffer"<< monsterattack<< " hp "<< totalhealth<< endl;
                Sleep(2000);
                CombactHud();
                Combact();
            }
        }
        else if(playerAttack == 3){
            cout<< "you try to run away"<<endl;
            int x = rand()%100 +1;
            if(x>=50){
                cout<<"You ran away"<<endl;
                Hud();
                Moving();
            }
            else{
                cout<<"You failed to run away"<<endl;
                cout<<"Monster Attacks"<< endl;
                totalhealth = totalhealth - monsterattack;
                cout<< "You suffer"<< monsterattack << " hp "<< totalhealth<< endl;
                CombactHud();
                Combact();
            }
        }
        if(playerAttack==4){
            pauseMenu();
        }
        else {
            cout<<"Invalid Number"<< endl;
            Combact();
        }
        
    }
    if(totalhealth<=1){
        system("cls");
        cout<<"You Died"<< "You were level "<< level<< "you got killed by " << currentMonster<< endl;
        Sleep(5000);
        exit(0); 
    }
    if(monsterHp<=1){
        Hud();
        xp += monsterxp;
        cout<<endl<<endl;
        cout<< "You defeated "<< currentMonster<< "you are rewarded with "<< monsterxp<< "xp, Good Job!"<<endl;
        counter = 0;
        Sleep(1500);
    }
    if(xp>=tolevel){
        level++;
        totalhealth = 100;
        totalhealth += 20 * level;
        maxhealth = totalhealth;
        cout<<"Nice you leveled up! You are now level "<< level<< std::endl;
        cout<< "Your total health increase by 20! Your max health is now "<< totalhealth << std::endl;
        Sleep(2000);
        Hud();
        tolevel= tolevel * level +1;
    }
    system("pause");
    Hud();
    Moving();
    system("pause");
    Hud();
    Moving();
}
    void CreatingMonster(){
        monsterHp = 30;
        monsterlevel = (rand()%3)+ level;
        if(monsterlevel == 0){
            monsterlevel = (rand()%3)+level;
        }
        monsterHp = (rand()%30) * monsterlevel;
        if(monsterHp == 0){
            monsterHp=(rand()%30) * monsterlevel;
        }
        monsterxp = monsterHp;
    }
    void Moving() {
        int choice;
        cout << std::endl;
        cout << "1. Open Map and Move" << endl; 
        cout << "2. Relax" << endl;
        cout << "3. Check Inventory" << endl;
        cout << "4. Pause Menu" << endl;
        cout << endl << endl;
        cin>> choice; 
        if (choice == 1) {
            showMap(); 
            Hud();
            Moving();
        }
        if(choice ==2){
            std::cout<< "You want to relax"<<std::endl;
            if(counter<=2){
                counter++;
                if(totalhealth<=99){
                    totalhealth += 10 * level;
                    if((totalhealth>100) && (level ==1)){
                        totalhealth = 100;
                    }
                    else if((totalhealth >120) && (level == 2)){
                        totalhealth = 120;
                    }
                    else if((totalhealth > 140) && (level =3)){
                        totalhealth = 140;
                    }
                    else if ((totalhealth > 160) && (level ==4)){
                        totalhealth = 160;
                    }
                }
                std::cout<< "You healed by resting. Health is now "<< totalhealth<<std::endl;
            }
            else {
                std::cout<< "You rested but did not heal. You feel stiff"<< std::endl;
            }
            Sleep(2000);
            Hud();
            Moving();
        }
        else if(choice ==3){
            int temp = rand()%100 +1;
            std::cout<< "You being to move forward.."<< std::endl;
            if(temp >=50){
                CreatingMonster();
                std::string temp = monstername[rand()%3];
                std::cout<< "A monster! it is a "<< temp<<std::endl;
                currentMonster = temp;
                Sleep(2000);
                Combact();
            }
            counter = 0;
        }
        else if(choice ==4){
            pauseMenu();
            return;
        }
        else if(choice ==5){
            showMap();
            return;
        }
        else if (choice == 6) {
            displayInventory();
            system("pause");
            Hud();
            Moving();
        }
        else{
            std::cout<< "Invalid Numer"<< std::endl;
            Sleep(2000);
            Hud();
            Moving();
        }
    }
    void Story(){
        std::string Intro_story = "You used to be a great hero one that most people looked up to. However one fatefull night as you were resting at an inn.\n"
        "You then were ambushed by the people of the city and you attempted to try to fight the them off only to be left for dead.However this wasnt the end as you had awaken in an alleyway\n"
        "You could feel the breeze as it feels like it could cut through your skin. You could hear the chatter of people pasing by and you can see that theres a path ahead. What do you want to do?\n";
        if(Intro){
            int i =0;
            while (Intro_story[i] != '\0'){
                std::cout<< Intro_story[i];
                Sleep(80);
                i++;
            }
            Intro = false;
            system("pause");
            CreateCharacter();
        }

        system("cls");
        std::cout<<"Clearing Console.";
        Sleep(200);
        system("cls");
        std::cout<<"Clearing Console.";
        Sleep(200);
        system("cls");
        std::cout<<"Clearing Console.";
        Sleep(200);
        system("cls");
        std::cout<<"Console cleared Resuming";
        Sleep(200);
        system("cls");
    }

void mainMenu() {
    int choice;
    system("cls");
    std::cout << "===== MAIN MENU =====" << std::endl;
    std::cout << "1. New Game" << std::endl;
    std::cout << "2. Load Game" << std::endl;
    std::cout << "3. Quit" << std::endl;
    std::cout << "=====================" << std::endl;
    std::cout << "Enter your choice: ";
    std::cin >> choice;

    switch (choice) {
        case 1:
            newGame();
            break;
        case 2:
            loadGame();
            break;
        case 3:
            quitGame();
            break;
        default:
            std::cout << "Invalid choice. Please try again." << std::endl;
            Sleep(1000);
            mainMenu();
    }
}

void newGame() {
    system("cls");
    std::cout << "Starting a new game..." << std::endl;
    Sleep(1000);
    level = 1;
    xp = 0;
    tolevel = 76;
    health = 100;
    totalhealth = health;
    maxhealth = totalhealth;
    Intro = true;
    Story();
    Hud();
    Moving();
}

void loadGame() {
    system("cls");
    std::ifstream saveFile("savegame.txt");
    if (saveFile.is_open()) {
        saveFile >> name >> race >> sex >> level >> xp >> totalhealth >> maxhealth >> tolevel;
        saveFile.close();
        std::cout << "Game loaded successfully!" << std::endl;
        Sleep(1000);
        Hud();
        Moving();
    } else {
        std::cout << "No save file found. Starting a new game instead." << std::endl;
        Sleep(1000);
        newGame();
    }
}

void quitGame() {
    system("cls");
    std::cout << "Are you sure you want to quit? (y/n): ";
    char choice;
    std::cin >> choice;
    if (choice == 'y' || choice == 'Y') {
        std::cout << "Goodbye!" << std::endl;
        Sleep(1000);
        exit(0);
    } else {
        mainMenu();
    }
}

void saveGame() {
    std::ofstream saveFile("savegame.txt");
    if (saveFile.is_open()) {
        saveFile << name << " " << race << " " << sex << " " << level << " " << xp << " " 
                 << totalhealth << " " << maxhealth << " " << tolevel;
        saveFile.close();
        std::cout << "Game saved successfully!" << std::endl;
    } else {
        std::cout << "Error saving game!" << std::endl;
    }
    Sleep(1000);
}

void shopMap() {
    system("cls");
    std::cout << "===== SHOP =====" << std::endl;
    std::cout << "1. Buy Potions (Restores 50 HP) - 20 gold" << std::endl;
    std::cout << "2. Buy Sword Upgrade (+5 damage) - 50 gold" << std::endl;
    std::cout << "3. Buy Armor (+20 max HP) - 40 gold" << std::endl;
    std::cout << "4. Leave Shop" << std::endl;
    std::cout << "================" << std::endl;
    
    int choice;
    std::cin >> choice;
    
    switch (choice) {
        case 1:
            cout << "You bought a potion!" <<endl;
           
            break;
        case 2:
            cout << "You upgraded your sword!" << endl;
            break;
        case 3:
            cout << "You bought armor!" << endl;
            break;
        case 4:
            default:
            Hud();
            Moving();
    }
    Sleep(1000);
    shopMap();
}

void explore() {
    system("cls");
    std::cout << "You explore the area..." << std::endl;
    int encounter = rand() % 100;
    if (encounter < 30) {
        CreatingMonster();
        std::string temp = monstername[rand() % 4];
        std::cout << "You encountered a " << temp << "!" << std::endl;
        currentMonster = temp;
        Sleep(2000);
        Combact();
    } else if (encounter < 60) {
        cout << "You found nothing of interest." << endl;
    } else if (encounter < 80) {
        cout << "You found a hidden path!" << endl;
       
    } else {
        cout << "You found a treasure chest!" << endl;
    }
    Sleep(2000);
    Hud();
    Moving();
}

void npcInteraction() {
    system("cls");
    std::cout << "You meet a friendly traveler." << std::endl;
    std::cout << "1. Talk" << std::endl;
    std::cout << "2. Trade" << std::endl;
    std::cout << "3. Ignore" << std::endl;
    
    int choice;
    std::cin >> choice;
    
    switch (choice) {
        case 1:
            std::cout << "Traveler: 'Be careful out there, dangerous creatures roam these lands.'" << std::endl;
            break;
        case 2:
            std::cout << "The traveler shows you some wares..." << std::endl;
            break;
        case 3:
        default:
            std::cout << "You ignore the traveler and move on." << std::endl;
    }
    Sleep(2000);
    Hud();
    Moving();
}

void questSystem() {
    system("cls");
    static bool questActive = false;
    static bool questCompleted = false;
    
    if (!questActive && !questCompleted) {
        std::cout << "Village Elder: 'We need your help! Monsters have been attacking our village.'" << std::endl;
        std::cout << "1. Accept quest" << std::endl;
        std::cout << "2. Decline" << std::endl;
        
        int choice;
        std::cin >> choice;
        
        if (choice == 1) {
            questActive = true;
            cout << "Village Elder: 'Thank you! Please defeat 3 monsters to help us.'" << endl;
        } else {
            cout << "Village Elder: 'Very well, maybe another time.'" << endl;
        }
    } else if (questActive) {
        static int monstersDefeated = 0;
        if (monstersDefeated < 3) {
            cout << "Quest: Defeat " << (3 - monstersDefeated) << " more monsters." << endl;
        } else {
            cout << "Village Elder: 'Thank you for saving our village! Here's your reward.'" << endl;
            xp += 50; 
            questActive = false;
            questCompleted = true;
        }
    } else if (questCompleted) {
        std::cout << "Village Elder: 'Thank you again for your help!'" << std::endl;
    }
    
    Sleep(2000);
    Hud();
    Moving();
}

void initializeMap() {
    for(int y = 0; y < Map_Size; y++) {
        for(int x = 0; x < Map_Size; x++) {
            gameMap[y][x] = 'P'; 
        }
    }
    gameMap[2][2] = 'C'; 
    gameMap[5][8] = 'F'; 
    gameMap[7][3] = 'M'; 
    gameMap[4][4] = 'L'; 
    gameMap[8][1] = 'T'; 
    playerX = 8;
    playerY = 1;
}

void showMap() {
    system("cls");
    cout << "=== WORLD MAP ===\n";
    cout << "Legend:\n";
    cout << "C = Castle, F = Forest\n";
    cout << "M = Mountains, L = Lake\n";
    cout << "T = Town, P = Plains\n";
    cout << "@ = Your position\n\n";
    cout << "Your position: (" << playerX << ", " << playerY << ")\n\n";
    for(int y = 0; y < Map_Size; y++) {
        for(int x = 0; x < Map_Size; x++) {
            if(x == playerX && y == playerY) {
                cout << "@ "; 
            } else {
                cout << gameMap[y][x] << " "; 
            }
        }
        cout << endl;
    }
    cout << "\n1. Move North";
    cout << "\n2. Move East";
    cout << "\n3. Move South";
    cout << "\n4. Move West";
    cout << "\n5. View Location Info";
    cout << "\n6. Return to Game";
    cout << "\nEnter choice: ";
    
    int choice;
    cin >> choice;
    
    switch(choice) {
        case 1: 
            if(playerY > 0) playerY--;
            break;
        case 2: 
            if(playerX < Map_Size-1) playerX++;
            break;
        case 3: 
            if(playerY < Map_Size-1) playerY++;
            break;
        case 4: 
            if(playerX > 0) playerX--;
            break;
        case 5:
            viewLocationInfo();
            break;
        case 6:
            return;
        default:
            cout << "Invalid choice!";
            Sleep(1000);
    }
    checkMapEncounter();
    showMap();
}
void viewLocationInfo() {
    system("cls");
    char currentLoc = gameMap[playerY][playerX];
    cout << "Location: ";
    switch(currentLoc) {
        case 'C': 
            cout << "Castle (Safe Zone)\n";
            cout << "You can rest and resupply here.\n";
            break;
        case 'M': 
            cout << "Rocky Mountains (Hard terrain)\n";
            cout << "Dangerous paths, but rich in minerals.\n";
            break;
        case 'T': 
            cout << "Home Town (Rest and shop)\n";
            cout << "Friendly faces and warm beds.\n";
            break;
        case 'F':
            cout << "Dark Forest\n";
            cout << "Thick foliage blocks sunlight.\n";
            break;
        case 'L':
            cout << "Crystal Lake\n";
            cout << "The water looks drinkable.\n";
            break;
        default: 
            cout << "Open Plains\n";
            cout << "Gentle rolling hills as far as the eye can see.\n";
    }
    system("pause");
}

void checkMapEncounter() {
    if(rand() % 100 < 30) {
        char terrain = gameMap[playerY][playerX];
        int encounterType = rand() % 100;
        
        if(terrain == 'T' || terrain == 'C') {
            npcInteraction(); 
        }
        else if(encounterType < 60) { 
            CreatingMonster();
            currentMonster = monstername[rand() % 4];
            cout << "A wild " << currentMonster << " appears!\n";
            system("pause");
            Combact();
        }
        else if(encounterType < 90) { 
            npcInteraction();
        }
        else { 
            findRandomItem();
        }
    }
}