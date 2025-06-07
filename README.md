# 🏦 Bank Management System

A comprehensive console-based banking management system built with C++ that handles client accounts, user management, and financial transactions with robust permission controls.

## ✨ Features

### 👥 Client Management
- **Add New Clients**: Create bank accounts with details (Account Number, PIN, Name, Phone, Balance)
- **View Clients**: Display all clients in formatted tables
- **Update Clients**: Modify existing client information
- **Delete Clients**: Remove client records
- **Find Clients**: Search for specific clients by account number

### 👨‍💼 User Management
- **User Roles**: 7 distinct permission levels
- **Add/Remove Users**: Full user account management
- **Permission Control**: Granular access control using bitmask system

### 💰 Transactions
- **Deposits**: Add funds to accounts
- **Withdrawals**: Remove funds (with balance validation)
- **Balance Reports**: View total balances across all accounts

### 🔒 Security
- **Login Authentication**: Username/password protected
- **Permission-Based Access**: Menu items dynamically enabled based on permissions
- **Data Validation**: Comprehensive input validation

## 🛠️ Technical Implementation

### 📁 Data Storage
- **Client Data**: `Client.txt` (structured text format)
- **User Data**: `Users.txt` (structured text format)

### 🧩 Data In Structure
```cpp
struct stClient {
    string AccountNumber;
    int PinCode;
    string Name;
    long long Phone;
    double AccountBalance;
    bool MarkFoDeleted;
};

struct stUser {
    string UserName;
    string Password;
    int Permessions; // Bitmask permissions
    bool MarkForDelete;
};
