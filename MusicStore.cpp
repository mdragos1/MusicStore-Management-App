#include <iostream>
#include <cstring>
#include <vector>
#include <sstream>
#include <map>
using namespace std;

class Product{
protected:
    string name;
    double price;
public:
    Product(const string &name="", double price=0) : name(name), price(price) {}  // asa nu mai am nevoie si de constructorul fara parametrii

    Product(Product const &p) : name(p.name), price(p.price) {}

    const string &getName() const {
        return name;
    }

    virtual bool isCompatibleWith(Product *other){
        return 0;
    }

    virtual istream& read(istream& is){
        cout << "Name: ";  is.get(); getline(is, name);
        cout << "Price: "; is >> price;
        return is;
    }
    virtual ostream& write(ostream& os)const{
        os << "\nName: "  << name;
        os << "\nPrice: " << price;
        return os;
    }

    friend istream &operator>>(istream &is, Product &product) {
        product.read(is);
        return is;
    }

    friend ostream &operator<<(ostream &os,const Product &product) {
        product.write(os);
        os<<"\n\n";
        return os;
    }

};

class Song: public Product {
    vector<string> authors;
    string title;
    int length;
    bool songType; // 0 - Vinyl | 1 - CD
public:
    Song(): Product(), title(""), length(0), songType(0) {}

    Song(const string &name, double price, const vector<string> &authors, const string &title, int length, bool songType) :
            Product(name, price), authors(authors), title(title), length(length), songType(songType) {}

    Song(const Song& s): Song(s.name, s.price, s.authors, s.title, s.length, s.songType) {}

    bool isCompatibleWith(Product *other) override ;

    const vector<string> &getAuthors() const;
    bool getSongType() const;
    const string &getTitle() const {
        return title;
    }

    istream &read(istream &is) override {
        Product::read(is);
        cout << "Title: "; is.get(); getline(is, title);
        cout << "Number of authors: "; int n; is >> n;
        for(int i=0; i<n; i++){
            cout << "Author " << i+1 << " : ";
            string author; is.get(); getline(is, author);
            authors.push_back(author);
        }
        cout << "Length: "; is >> length;
        cout << "Song Type(0-Vinyl | 1-CD): "; is >> songType;
        return is;
    }

    ostream &write(ostream &os) const override {
        Product::write(os);
        os << "\nTitle: " << title;
        os << "\nNumber of authors: " << authors.size();
        for(int i=0; i<authors.size(); i++){
            os << "\nAuthor " << i+1 << " : " << authors[i];
        }
        os << "\nLength: " << length;
        os << "\nSong Type: ";
        if(songType==0)
            os << "Vinyl";
        else
            os << "CD";
        return os;
    }
};

class MusicPlayer: public Product{
    string title, singer;
    bool playerType; //0-VinylPLayer | 1-CdPLayer
public:

    bool getPlayerType() const {
        return playerType;
    }

    const string &getTitle() const {
        return title;
    }

    const string &getSinger() const {
        return singer;
    }

    MusicPlayer(const string &name="", double price=0, const string &title="", const string &singer="", bool playerType=0) :
                            Product(name, price),title(title), singer(singer), playerType(playerType) {}

    MusicPlayer(const MusicPlayer& m): MusicPlayer(m.name, m.price, m.title, m.singer, m.playerType) {}

    istream &read(istream &is) override {
        Product::read(is);
        cout << "Title: "; is.get(); getline(is, title);
        cout << "Singer: "; is.get(); getline(is, singer);
        cout << "Player Type(0-VinylPlayer | 1-CdPlayer): "; is >> playerType;
        return is;
    }

    ostream &write(ostream &os) const override {
        Product::write(os);
        os << "\nTitle: " << title;
        os << "\nSinger: " << singer;
        os << "\nPlayer Type: ";
        if(playerType==0)
            os << "VinylPLayer";
        else
            os << "CdPlayer";
        return os;
    }

    bool isCompatibleWith(Product *other) override;
};

class Sticker: public Product{
    string text;
public:

    const string &getText() const {
        return text;
    }

    Sticker(const string &name="", double price=0, const string &text="") : Product(name, price), text(text) {}

    istream &read(istream &is) override {
        Product::read(is);
        cout << "Text: "; is.get(); getline(is,text);
        return is;
    }

    ostream &write(ostream &os) const override {
        Product::write(os);
        os << "\nText: " << text;
        return os;
    }

    bool isCompatibleWith(Product *other) override;

};

string getTypeOfProduct(Product* p){ //1-song 2-musicPlayer 3-sticker
    if(dynamic_cast<Song*>(p) != nullptr)
        return "Song ("+ p->getName() + ")" ;
    else if(dynamic_cast<MusicPlayer*>(p) != nullptr)
        return "MusicPlayer (" + p->getName() + ")" ;
    else if(dynamic_cast<Sticker*>(p) != nullptr)
        return "Sticker (" + p->getName() + ")" ;
    return "";
}
int getTypeOfProduct2(Product* p){ //1-song 2-musicPlayer 3-sticker
    if(dynamic_cast<Song*>(p) != nullptr)
        return 1 ;
    else if(dynamic_cast<MusicPlayer*>(p) != nullptr)
        return 2 ;
    else if(dynamic_cast<Sticker*>(p) != nullptr)
        return 3;
    return 0;
}


class MusicStore{
private:
    vector<Product*> products;
    static MusicStore* app;
    MusicStore() {}
public:
    const vector<Product *> &getProducts() const {
        return products;
    }

    static MusicStore *getInstance() {
        if (!app) {
            app = new MusicStore;
        }
        return app;
    }



    MusicStore(const vector<Product*> &products): products(products){}

    virtual ~MusicStore() {
        for(auto *i: products)
            delete i;
    }

    void addProduct(){
        cout << "\nWhat type of product would you like to add?";
        cout << "\n1 - Song | 2 - Music Player | 3 - Sticker";
        int input; cout << "\nInput: "; cin >> input;
        Product* obj = 0;
        if(input == 1){
            obj = new Song;
        }else if(input == 2){
            obj = new MusicPlayer;
        }else if(input == 3){
            obj = new Sticker;
        }
        if (obj)
            cin >> *obj;
        products.push_back(obj);
    }

    void addProduct(const Product& p){
        int c=0;
        Product* obj=0;
        try{auto asSong = dynamic_cast<const Song&>(p);
            obj = new Song(asSong);
        }catch (bad_cast&) {c++;}

        try {auto asMusicPlayer = dynamic_cast<const MusicPlayer&>(p);
            obj = new MusicPlayer(asMusicPlayer);
        }catch (bad_cast&) {c++;}


        try{auto asSticker = dynamic_cast<const Sticker&>(p);
            obj= new Sticker(asSticker);
        }
        catch (bad_cast&) {c++;}
        if(c==3)
            cout << "This type of product cannot be added";
        products.push_back(obj);
    }

    template<class product>void addProductBonus(const product& p) {  ///idee bonus, mi se pare mult mai usor de
        Product* obj;                                                ///implementat decat metoda de mai sus
        obj=new product;
        *obj=p;
        products.push_back(obj);
    }

    void deleteProduct() {
        if(products.size()==0)
            cout << "\nThe store is empty. There is no product that can be deleted.\n";
        else{
            cout << "Type in the number of the product you want to delete. \n";
            cout << "Input: ";
            int n;
            cin >> n;
            if (n >= 1 && n <= products.size()) {
                products.erase(products.begin()+n-1);
            }
            else
                cout << "There isn't a product available at that number. " << n << "\n";
        }
    }


    friend ostream &operator<<(ostream &os, const MusicStore &store) {
        os << "\nProducts: ";
        for(int i=0; i<store.products.size(); i++){
            os << "\nProduct number " << i+1 <<":\n" << *store.products[i];
        }
        return os;
    }
    friend istream &operator>>(istream &is,  MusicStore &store) {
        cout << "\nNumber of products: ";
        int in;is>>in;
        Product* obj = 0;
        for(int i=0; i<in; i++){
            cout << "\nProduct number " << i+1 <<":\n";
            cout << "\nWhat type of product would you like to add?";
            cout << "\n1 - Song | 2 - Music Player | 3 - Sticker";
            int input; cout << "\nInput: "; is >> input;
            if(input == 1){
                obj = new Song;
            }else if(input == 2){
                obj = new MusicPlayer;
            }else if(input == 3){
                obj = new Sticker;
            }
        }
        store.products.push_back(obj);
        return is;
    }


    void printProduct(){
        if(products.size()==0){
            cout << "\nThe store is empty. There is no product available.\n";
        }else
            cout << *this;
    }
    friend void group(const MusicStore &ms);

    void showMenu(){
        cout << "\n1. See available products" << endl;
        cout << "2. Add a product" << endl;
        cout << "3. Delete a product" << endl;
        cout << "4. Show compatible products" << endl;
        cout << "5. Group product by categories" << endl;
        cout << "6. Exit" << endl;
        cout << "Input: ";
    }

    void run() {
        while (true) {
            showMenu();
            int input;
            cin >> input;
            if (input == 1) {
                printProduct();
            }else if(input==2){
                addProduct();
            }else if(input==3){
                deleteProduct();
            }else if(input==4){
                compare();
            }else if(input==5){
                group(*this);
            }else if (input == 6) { //// EXIT
                break;
            }
        }
    }

    void compare(){
        if(products.size()==0){
            cout << "\nThe store is empty. There are no products that can be compared.\n";
        }else
            for(int i =0;i<products.size()-1;i++){
                for(int j=0; j<products.size();j++)
                    if(products[i]->isCompatibleWith(products[j])==1){
                        cout << "\n" << getTypeOfProduct(products[i]) << " is compatible with " << getTypeOfProduct(products[j]);
                    }
            }
    }
};

MusicStore* MusicStore::app;

vector<string> word1, word2;
void getWords(string str, vector<string> &words )
{
    words.clear();
    // word variable to store word
    string word;

    // making a string stream
    stringstream iss(str);

    // Read and push back each word.
    while (iss >> word)
       words.push_back(word);
}

bool Song:: isCompatibleWith(Product *other)  {
    if(dynamic_cast<Song*>(other) != nullptr){
        auto *s = dynamic_cast<Song*>(other);                   //// verific daca doua cantece au aceiasi autori
        if(this->authors==s->authors){
            return 1;
        }
    } else if(dynamic_cast<MusicPlayer*>(other) != nullptr){
        auto *mp = dynamic_cast<MusicPlayer*>(other);           ////verific daca ambele sunt CD sau Vinyl
        if(songType==mp->getPlayerType())
            return 1;
    } else if(dynamic_cast<Sticker*>(other) != nullptr){        ////fac pentru fiecare string (sau vector de string) din song vectorul de cuvinte in word1
        auto *st =dynamic_cast<Sticker*>(other);                ////si verific daca are vreun cuvant din mesajul stickerelui
        getWords(name, word1);
        getWords(st->getText(), word2);
        for(auto i: word1){
            for(auto j:word2)                              ///verific  cuvant din nume == cuvant din mesaj
                if(i==j)
                    return 1;
        }
        getWords(title, word1);
        for(auto i: word1){
            for(auto j:word2)                             ///verific  cuvant din titlu == cuvant din mesaj
                if(i==j)
                    return 1;
        }
        for(auto autor: authors){
            getWords(autor, word1);
            for(auto i: word1){                           ///pentru fiecare autor verific cuvant din autor == cuvant din mesaj
                for(auto j:word2)
                    if(i==j)
                        return 1;
            }
        }
    }
    return 0;
}

bool MusicPlayer::isCompatibleWith(Product *other) {
    if(dynamic_cast<Song*>(other) != nullptr){
        auto *s = dynamic_cast<Song*>(other);
        if(playerType== s->getSongType()){                ////verific daca sunt ambele CD sau ambele Vinyl
            return 1;
        }
    }else if(dynamic_cast<Sticker*>(other) != nullptr){   ////fac pentru fiecare string din musicPlayer vectorul de cuvinte in word1
        auto *st =dynamic_cast<Sticker*>(other);          ////si verific daca are vreun cuvant din mesajul stickerelui
        getWords(name, word1);
        getWords(st->getText(), word2);
        for(auto i: word1){
            for(auto j:word2)                             ///verific  cuvant din nume == cuvant din mesaj
                if(i==j)
                    return 1;
        }
        getWords(title, word1);
        for(auto i: word1){
            for(auto j:word2)                             ///verific  cuvant din titlu == cuvant din mesaj
                if(i==j)
                    return 1;
        }
        getWords(singer, word1);
        for(auto i: word1){
            for(auto j:word2)                             ///verific  cuvant din singer == cuvant din mesaj
                if(i==j)
                    return 1;
        }
    }
    return 0;

}

bool Sticker::isCompatibleWith(Product *other) {
    if(dynamic_cast<Song*>(other) != nullptr){             ////fac pentru fiecare string (sau vector de string) din song vectorul de cuvinte in word2
        auto *s =dynamic_cast<Song*>(other);               ////si verific daca are vreun cuvant din mesajul stickerelui
        getWords(text, word1);
        getWords(s->getName(), word2);
        for(auto i: word1){
            for(auto j:word2)                              ///verific  cuvant din nume == cuvant din mesaj
                if(i==j)
                    return 1;
        }
        getWords(s->getTitle(), word2);
        for(auto i: word1){
            for(auto j:word2)                             ///verific  cuvant din titlu == cuvant din mesaj
                if(i==j)
                    return 1;
        }
        for(auto autor: s->getAuthors()){
            getWords(autor, word2);
            for(auto i: word1){                           ///pentru fiecare autor verific cuvant din autor == cuvant din mesaj
                for(auto j:word2)
                    if(i==j)
                        return 1;
            }
        }
    } else if(dynamic_cast<MusicPlayer*>(other) != nullptr){ ////fac pentru fiecare string din musicPlayer vectorul de cuvinte in word2
        auto *m =dynamic_cast<MusicPlayer*>(other);          ////si verific daca are vreun cuvant din mesajul stickerelui
        getWords(text, word1);
        getWords(m->getName(), word2);
        for(auto i: word1){
            for(auto j:word2)                               ///verific  cuvant din nume == cuvant din mesaj
                if(i==j)
                    return 1;
        }
        getWords(m->getTitle(), word2);
        for(auto i: word1){
            for(auto j:word2)                              ///verific  cuvant din titlu == cuvant din mesaj
                if(i==j)
                    return 1;
        }
        getWords(m->getSinger(), word2);
        for(auto i: word1){
            for(auto j:word2)                             ///verific  cuvant din singer == cuvant din mesaj
                if(i==j)
                    return 1;
        }
    }
    return 0;
}

const vector<string> &Song::getAuthors() const {
    return authors;
}

bool Song::getSongType() const {
    return songType;
}

struct ProductCategory {
    string type;
public:
    ProductCategory(string type):type(type){}
    ProductCategory(const ProductCategory& pc) : type(pc.type){}

    ProductCategory() {}
};

const ProductCategory SONG("SONG"), MUSIC_PLAYER("MUSIC_PLAYER"), STICKER("STICKER");
//
//map<const ProductCategory, Product**> groupProducts(const MusicStore &s){
//    Product** products;
//
//    products = new Product*[50];
//    int index=0;
//    for(auto *i : s.getProducts()){
//        products[index] = i;
//        index++;
//    }
//    map<const ProductCategory, Product**> group;
//    for (auto i=0;i<index;i++){
//        if(getTypeOfProduct2(products[i])==1){
//            group[SONG] = products;
////            group.insert(make_pair(SONG,products));
//        }
//    }
//    return group;
//}


map<Product*, ProductCategory> groupProducts(const MusicStore &s) {
    Product* product;
    map<Product*,  ProductCategory> group;
    for(auto i : s.getProducts()){
        if(getTypeOfProduct2(i)==1){
            auto a=dynamic_cast<Song&>(*i);
            product=new Song(a);
//            group[product] = SONG;
            group.insert(pair<Product*, ProductCategory>(product,SONG));
        }else if(getTypeOfProduct2(i)==2){
            auto a=dynamic_cast<MusicPlayer&>(*i);
            product=new MusicPlayer(a);
            group.insert(pair<Product*, ProductCategory>(product,MUSIC_PLAYER));
        }else if(getTypeOfProduct2(i)==3){
            auto a=dynamic_cast<Sticker&>(*i);
            product=new Sticker(a);
            group.insert(pair<Product*, ProductCategory>(product,STICKER));
        }
    }
    return group;
}

void group(const MusicStore &ms){
    map<Product*, ProductCategory> group = groupProducts(ms);
    map<Product*, ProductCategory>::iterator itr;
    cout << "\nChoose the category of products you want to see. \n 1-Songs | 2-MusicPlayers | 3-Stickers \nInput:";
    int in; cin>>in;
    if(in==1){
        cout << "\n Current available songs: ";
        for (itr = group.begin(); itr != group.end(); ++itr) {
            if(itr->second.type=="SONG"){
                cout << "\n" << itr->first->getName() << "\t";
            }
        }
    }else if (in==2) {
        cout << "\n Current available music players: ";
        for (itr = group.begin(); itr != group.end(); ++itr) {
            if (itr->second.type == "MUSIC_PLAYER") {
                cout << "\n" << itr->first->getName() << "\t";
            }
        }
    }else if (in==3) {
        cout << "\n Current available stickers: ";
        for (itr = group.begin(); itr != group.end(); ++itr) {
            if (itr->second.type == "STICKER") {
                cout << "\n" << itr->first->getName() << "\t";
            }
        }
    }
}

int main() {
    Song m("merry", 40,{"Ariana", "Grande"},"Thank you, next", 260,1);
    Sticker m2("StickerCraciun", 15, "merry christmas everyone");
    Song m3("merry christmas", 69,{"Ariana", "Grande"},"34 35", 300,1);
    MusicPlayer m4("Ipod", 200, "Mistletoe", "Justin Bieber", 1);
    Sticker m5("StickerPaste", 20, "Happy Easter");
    MusicStore* ms = MusicStore::getInstance();
    ms->addProduct(m);
    ms->addProduct(m2);
    ms->addProduct(m3);
    ms->addProduct(m4);
    ms->addProduct(m5);
//    map<Product*, ProductCategory> group = groupProducts(ms);
//    map<Product*, ProductCategory>::iterator itr;
//    cout << "\nThe map group is : \n";
//    for (itr = group.begin(); itr != group.end(); ++itr) {
//        cout << '\t' << itr->first->getName()
//             << '\t' << itr->second.type << '\n';
//    }
    ms->run();
    return 0;
}