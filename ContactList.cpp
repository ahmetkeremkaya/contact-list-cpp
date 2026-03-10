#include "ContactList.h"
#include <iostream>





ContactList::ContactList() {}

ContactList::~ContactList() {}

void ContactList::addContact(const std::string &name, const ContactInfo &info) {
    contacts.put(name,info);
}

bool ContactList::deleteContact(const std::string &name) {
    return contacts.deletekey(name);
}

void ContactList::updateContact(const std::string &name, const ContactInfo &newInfo) {
    contacts.update(name,newInfo);
}



const ContactInfo &ContactList::getContact(const std::string &name) const {
    return contacts.get(name);
}



void ContactList::iterateAlphabetically()  {
     if (contacts.size() == 0)
        return;

    // En küçük ve en büyük key'leri al (alfabetik olarak)
    std::string key     = contacts.firstEntry().getKey();
    std::string lastKey = contacts.lastEntry().getKey();

    while (true) {
        const ContactInfo& info = contacts.get(key);

        // ---- ÖDEV FORMATINA UYGUN ÇIKTI ----
        std::cout << "Name: " << key << "\n";

        std::cout << "Phones: ";
        for (size_t i = 0; i < info.phones.size(); ++i) {
            std::cout << info.phones[i];
            if (i + 1 < info.phones.size())
                std::cout << ", ";
        }
        std::cout << "\n";

        std::cout << "Email: " << info.email << "\n";
        std::cout << "Company: " << info.company << "\n";

        if (!info.notes.empty())
            std::cout << "Notes: " << info.notes << "\n";

        std::cout << "\n";
        // ------------------------------------

        // Eğer şu an son key'deysek, yazdırmayı bitir
        if (key == lastKey)
            break;

        // Bir SONRAKİ alfabetik key'i bulmak için:
        // key'den biraz büyük bir string oluştur → key + '\0'
        std::string searchKey = key;
        searchKey.push_back('\0');

        const KeyValuePair<std::string, ContactInfo>& nextPair =
    contacts.ceilingEntry(searchKey);
        key = nextPair.getKey();
    }
}

