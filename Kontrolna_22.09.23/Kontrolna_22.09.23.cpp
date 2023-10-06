#include <iostream>
#include <string>
#include <conio.h>
using namespace std;

enum Key { ESC = 27};

const string ANSI_RESET = "\x1B[0m";
const string ANSI_RED = "\x1B[31m";
const string ANSI_GREEN = "\x1B[32m";
const string ANSI_YELLOW = "\x1B[33m";
const string ANSI_LIGHT_PURPLE = "\x1B[95m";
// патерн Будівельник
class Laptop
{
private:
    string tipName;
    string screenResolution;
    string processor;
    string memory;
    string hdd;
    string battery;
    double priceInDollars;

public:
    Laptop() : priceInDollars(0.0) {}

    void SetTipName(string value) { tipName = value; }
    void SetResolution(string value) { screenResolution = value; }
    void SetProcessor(string value) { processor = value; }
    void SetMemory(string value) { memory = value; }
    void SetHDD(string value) { hdd = value; }
    void SetBattery(string value) { battery = value; }
    void SetPriceInDollars(double price) { priceInDollars = price; }

    double GetPriceInDollars() const { return priceInDollars; }

    void Show() const
    {
        cout << tipName << endl;
        cout << ANSI_LIGHT_PURPLE <<"\tScreen Resolution :: " << screenResolution << endl;
        cout << "\tProcessor         :: " << processor << endl;
        cout << "\tMemory            :: " << memory << endl;
        cout << "\tHDD               :: " << hdd << endl;
        cout << "\tBattery           :: " << battery << endl;
        cout << "\tPrice in Dollars  :: $" << ANSI_RESET << priceInDollars << endl;
    }
};

class LaptopBuilder
{
protected:
    Laptop* laptop;

public:
    LaptopBuilder() : laptop(nullptr) {}

    void CreateLaptop()
    {
        laptop = new Laptop();
    }

    Laptop* GetLaptop()
    {
        return laptop;
    }

    virtual void SetSpecifications() = 0;
};

class GamingLaptopBuilder : public LaptopBuilder
{
public:
    void SetSpecifications() override
    {
        laptop->SetTipName("\n\t << Gaming  Laptop  Specifications   [1] >>\n");
        laptop->SetResolution("1900x1200");
        laptop->SetProcessor("Intel Core i7, 2.9GHz");
        laptop->SetMemory("32 GB");
        laptop->SetHDD("1 TB");
        laptop->SetBattery("5000 mAh");
        laptop->SetPriceInDollars(1200.0);
    }
};

class ProgrammerLaptopBuilder : public LaptopBuilder
{
public:
    void SetSpecifications() override
    {
        laptop->SetTipName("\n\t << Programmer Laptop Specifications [2] >>\n");
        laptop->SetResolution("2000x1200");
        laptop->SetProcessor("Intel Core i9, 2.9GHz");
        laptop->SetMemory("64 GB");
        laptop->SetHDD("1 TB");
        laptop->SetBattery("7000 mAh");
        laptop->SetPriceInDollars(1500.0);
    }
};

class LaptopDirector
{
    LaptopBuilder* builder;

public:
    LaptopDirector(LaptopBuilder* builder = nullptr) : builder(builder) {}

    ~LaptopDirector()
    {
        delete builder;
    }

    void SetBuilder(LaptopBuilder* builder)
    {
        if (this->builder != nullptr)
            delete this->builder;
        this->builder = builder;
    }

    Laptop* GetLaptop()
    {
        return builder->GetLaptop();
    }

    void ConstructLaptop()
    {
        builder->CreateLaptop();
        builder->SetSpecifications();
    }
};
// патерн Адаптer
class DollarPayment
{
public:
    void payInDollars(double amount)
    {
        cout << "Paid $" << amount << endl;
    }
};

class PaymentAdapter : public DollarPayment
{
private:
    double exchangeRate;

public:
    PaymentAdapter(double exchangeRate) : exchangeRate(exchangeRate) {}

    void payInHryvnia(double amountInDollars)
    {
        double amountInHryvnia = amountInDollars * exchangeRate;
        cout << ANSI_RED << " Paid $" << amountInDollars << " using the exchange rate of " << exchangeRate << ": " << amountInHryvnia << " UAH" << ANSI_RESET << endl;
    }
};

// патерн Фасад
class Facade
{
private:
    LaptopDirector gamingDirector;
    LaptopDirector programmerDirector;
    PaymentAdapter* adapter;
    double price;
    Laptop* laptop;  // Додати поле для зберігання ноутбука

public:
    Facade(double exchangeRate) : price(0), laptop(nullptr)
    {
        // Створюємо адаптер з обмінним курсом
        adapter = new PaymentAdapter(exchangeRate);

        // Ініціалізуємо директорії та будівельники
        gamingDirector.SetBuilder(new GamingLaptopBuilder());
        programmerDirector.SetBuilder(new ProgrammerLaptopBuilder());
    }

    ~Facade()
    {
        delete adapter;
        // Звільнюємо ноутбук під час знищення об'єкта Facade
        delete laptop;
    }

    void PurchaseGamingLaptop()
    {
        // Створюємо і конфігуруємо ігровий ноутбук
        gamingDirector.ConstructLaptop();
        laptop = gamingDirector.GetLaptop();  // Зберегти ноутбук

        // Виводимо інформацію про ноутбук
        laptop->Show();
        cout << ANSI_GREEN <<" ===================================================" << ANSI_RESET << endl;
    }

    void PurchaseProgrammerLaptop()
    {
        // Створюємо і конфігуруємо ноутбук для програміста
        programmerDirector.ConstructLaptop();
        laptop = programmerDirector.GetLaptop();  // Зберегти ноутбук

        // Виводимо інформацію про ноутбук
        laptop->Show();
        cout << ANSI_GREEN <<" ===================================================" << ANSI_RESET << endl;
    }
    void Payment()
    {
        cout << ANSI_YELLOW <<"\t\t[1] Gaming     Laptop" << ANSI_RESET << endl;
        cout << ANSI_YELLOW <<"\t\t[2] Programmer Laptop" << ANSI_RESET << endl;
        cout << ANSI_YELLOW <<"\t    Select the laptop to purchase:" << ANSI_RESET << endl;
        cout << "\t\t\t  ";
        int choice;
        cin >> choice;

        switch (choice)
        {
        case 1:
            PurchaseGamingLaptop();
            if (laptop != nullptr)
            {
                double priceInDollars = laptop->GetPriceInDollars();
                adapter->payInHryvnia(priceInDollars);
            }
            break;
        case 2:
            PurchaseProgrammerLaptop();
            if (laptop != nullptr)
            {
                double priceInDollars = laptop->GetPriceInDollars();
                adapter->payInHryvnia(priceInDollars);
            }
            break;
        default:
            cout << "    Invalid choice. Please select a valid option." << ANSI_RESET << endl;
            break;
        }
    }
};
int main()
{
    Facade facade(38.0);
    facade.PurchaseGamingLaptop(); 
    facade.PurchaseProgrammerLaptop();
    cout << ANSI_YELLOW << "\t   Press the [SPACE] button to pay."  << endl;
    cout << "\t    Click the button [ESC] to exit" << ANSI_RESET << endl;
    while (!_kbhit())
    {
        Key a = static_cast<Key>(_getch());

        if (a != ESC)
        {
            // Оплата 
            facade.Payment();
            cout << ANSI_YELLOW << "\t   Press the [SPACE] button to pay." << endl;
            cout << "\t    Click the button [ESC] to exit" << ANSI_RESET << endl;
        }
        else
        {
            break;
        }
    }
}
