#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include <random>
#include <algorithm>

using namespace std;

#define  INITNUM  2     //初めに配られるカードの枚数

class Chip
{
public:
    int chips;
    Chip()
    {
        chips = 0;
    }
    Chip(int ch)
        : chips(ch)
    {}

    int chip_num()
    {
        return chips;
    }

    void decrease_chips(int ch)
    {
        chips -= ch;
    }

    void increase_chips(int ch)
    {
        chips += ch;
    }

    bool bet_able(int ch)
    {
        if (chips < ch)
            return false;
        else 
            return true;
    }
};

class Deck
{
    public:
        vector<pair<char, int> > cards;

        //52枚のトランプカードをセットする。D:ダイア, H:ハート, S:スペード, C:クローバー         
        Deck()
        {
            for (int i =1; i <= 13; i ++)
            {
                for (int j = 0; j < 4; j ++)
                {
                    switch (j)
                    {
                        case 0:   cards.push_back(make_pair('H', i));   break;
                        case 1:   cards.push_back(make_pair('S', i));   break;
                        case 2:   cards.push_back(make_pair('D', i));   break;
                        case 3:   cards.push_back(make_pair('C', i));   break;
                    }
                }
            }
        }

        //デッキをシャッフルする。
        void shuffle_cards()
        {
            random_device seed_gen;
            mt19937 engine(seed_gen());
            shuffle(cards.begin(), cards.end(), engine);
           
        }

        //デッキの一番上（配列の末端の要素）を返す。
        pair<char, int> top_card()
        {
            pair<char, int> c = cards[cards.size() - 1];
            return c;
        }

        //デッキの一番上（配列の末端の要素）を削除する。
        void delete_top()
        {
            cards.pop_back();
        }

        //デッキの枚数を返す。
        int cards_num()
        {   
            return cards.size();
        }

        //デッキのカードをすべて表示
        void print_cards()
        {
            for (int i = 0; i < cards.size(); i ++)
            {
                cout << cards[i].first << cards[i].second << endl;
            }
            cout << this->cards_num() << endl;
        }
};

class Hand
{
    public:
        vector<pair<char, int> > hand;

        //ハンドにAがあるか否か
        bool is_A()
        {
            for (int i = 0; i < hand.size(); i ++)
            {
                if (hand[i].second == 1)
                {
                    return true;
                }
            }
            return false;
        }


        //ハンドの点数を返す。
        int calc()
        {
            int sum = 0;
            for (int i = 0; i < hand.size(); i++)
            {
                if (hand[i].second > 10)
                {
                    sum += 10;
                    continue;
                }
                sum += hand[i].second;
            }

            if (is_A() && sum <= 11)
            {
                sum += 10;
            }

            return sum;
        }

        //ハンドがブラックジャックであるか否か
        bool is_blackjack()
        {
            if (hand.size() == 2 && calc() == 21)
            {
                return true;
            }
            return false;
        }

};


class Player : public Hand
{
    
    public:
        Chip ch;
        Player(Deck& d, int n)
                        : ch(n)
        {                   
            int i = INITNUM;
            while(i--)
            {
                hand.push_back(d.top_card());
                d.delete_top();
            }
            
        }

        //プレイヤーのハンドを表示
        void print_hand()
        {
            cout << "--あなたのハンド--" << endl;
            for (int i = 0; i < hand.size(); i ++)
                cout << hand[i].first << hand[i].second << " ";
            cout << endl;
        }

        void print_chip()
        {
            cout << "あなたは" << ch.chip_num() << "チップ持っています。" << endl;
        }
       

};

class Deeler : public Hand
{
    Deck* deck;
    public:
        Deeler(Deck& d)
        {
            int i = INITNUM;
            while(i--)
            {
                hand.push_back(d.top_card());
                d.delete_top();
            }

            deck = &d;
            
        }

        void first_open()
        {
            cout << hand[0].first << hand[0].second << " " << "?" << endl;
        }

        void print_hand()
        {
            cout << "--ディーラーのハンド--" << endl;
            for (int i = 0; i < hand.size(); i ++)
                cout << hand[i].first << hand[i].second << " ";
            cout << endl;
        }

        void deel(vector<pair<char, int> > &h)
        {
            h.push_back((*deck).top_card());
            deck->delete_top();
        }

        void print_deck()
        {
            for (int i = 0; i < (*deck).cards.size(); i ++)
            {
                cout << deck->cards[i].first << deck->cards[i].second << endl;
            }
            cout << deck->cards.size() << endl;
        }
       

};



class Game
{  
public:
    int bets;
    Deeler* deeler;
    Player* player;

    Game(Deeler &d, Player &p)
    {
        deeler = &d;
        player = &p;
    }

    void start()
    {
        cout << "カードが２枚配られます。" << endl;
        cout << "--ディーラーのハンド--" << endl;
        deeler->first_open();
        player->print_hand();
        cout << player->calc() << endl;

        
    }

    
    char hit_or_stand()
    {
        char s;
        cout << "--HIT or STAND--" << endl;
        cout << "HITならHをSTANDならSを入力してください。" << endl;
        cin >> s;
        return s;
    }

    void bet()
    {
        bool flag = true;
        while (flag)
        {
            int b = 0;
            player->print_chip();
            cout << "いくらBETしますか？(100刻み)";
            cin >> b;
            if( !player->ch.bet_able(b))
            {
                cout << "チップが足りません" << endl;
                continue;
            } else if (b < 100 || b % 100 != 0)
            {
                cout << "100点刻みでのみBET可能" << endl;
                continue;
            }
            flag = false;
            bets = b;
        }
        
    }

    void game_roop()
    {
        int cnt = 0;
        while(1)
        {
            if (player->is_blackjack())
            {
                cout << "Black Jack!!" << endl;
                cout << "ディーラーの２枚目のカードを開けます。" << endl;
                deeler->print_hand();
                cout << deeler->calc() << endl;
                if (deeler->is_blackjack())
                {
                    cout << "Black Jack!!" << endl;
                    cout << "引き分けです。";
                    break;
                }
                cout << "あなたの勝ちです。" << endl;
                player->ch.increase_chips(bets * 1.5);
                cout << "チップは" << player->ch.chip_num() << "点になりました。" << endl;
                break;
            }
            
            if (player->calc() > 21)
            {
                cout << "バーストしました。" << endl;
                cout << "あなたの負けです。" << endl;
                player->ch.decrease_chips(bets);
                cout << "チップは" << player->ch.chip_num() << "点になりました。" << endl;
                break;
            }

            if (cnt == 1)
            {
                cout << "ディーラーの２枚目のカードを開けます。" << endl;
                deeler->print_hand();
                cout << deeler->calc() << endl;
                if (deeler->is_blackjack())
                {
                    cout << "Black Jack!!" << endl;
                    cout << "あなたの負けです。" << endl;
                    player->ch.decrease_chips(bets);
                    cout << "チップは" << player->ch.chip_num() << "点になりました。" << endl;
                    break;
                }
            }

            cnt ++;

           
            char s = hit_or_stand();

            if (s == 'H')
            {
                cout << "カードが１枚配られます。" << endl;
                deeler->deel(player->hand);
                player->print_hand();
                cout << player->calc() << endl;
                continue;
            } 
            else if (s == 'S')
            {
                while(deeler->calc() < 17)
                {
                    cnt ++;
                    deeler->deel(deeler->hand);
                    deeler->print_hand();
                    cout << deeler->calc() << endl;
                }
                if (deeler->calc() > 21)
                {
                    cout << "ディーラーがバーストしました。" << endl;
                    cout << "あなたの勝ちです。" << endl;
                    player->ch.increase_chips(bets);
                    cout << "チップは" << player->ch.chip_num() << "点になりました。" << endl;
                    break;
                }
                judge(cnt);
                break;
            }
            else 
            {
                cout << "もう一度入力してください。" << endl;
            }
            
        }
    }

    void judge(int cnt)
    {
        int deeler_point = 21 - deeler->calc();
        int player_point = 21 - player->calc();
        if (player_point < deeler_point)
        {
            if (cnt == 1)
            {
                deeler->print_hand();
                cout << deeler->calc() << endl;
            }
            
            cout << "あなたの勝ちです。" << endl;
            player->ch.increase_chips(bets);
            cout << "チップは" << player->ch.chip_num() << "点になりました。" << endl;
        } else if (player_point > deeler_point)
        {

            if (cnt == 1)
            {
                deeler->print_hand();
                cout << deeler->calc() << endl;
            }
            cout << "あなたの負けです。" << endl;
            player->ch.decrease_chips(bets);
            cout << "チップは" << player->ch.chip_num() << "点になりました。" << endl;
        } 
        else
        {
            if (cnt == 1)
            {
                deeler->print_hand();
                cout << deeler->calc() << endl;
            }
            cout << "引き分けです。" << endl;
            cout << "チップは" << player->ch.chip_num() << "点になりました。" << endl;
        }
        
    }
};



int main()
{
    

    Deck deck1;
    deck1.shuffle_cards();

    //第二引数はチップ数
    Player hikaru(deck1, 5000);
    Deeler deeler(deck1);

    Game game(deeler, hikaru);
    
    game.bet();
    game.start();
    game.game_roop();
    
}
