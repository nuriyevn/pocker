#include <iostream>
#include <array>
#include <vector>
#include <random>
#include <ctime>
#include <list>

using namespace std;

//std::array<std::string, 13> RANK = { "A", "2", "3", "4", "5","6","7","8","9","10", "J", "K", "Q" };
enum SUIT { HEARTS=0, DIAMONDS, CLUBS, SPADES, SUIT_MAX };
enum RANK { ACE=0, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, KING, QUEEN, RANK_MAX };

enum COMPARISON_CRITERIA {EQUAL=0, CONSEUQUENT};

class Card
{
    RANK rank;
    SUIT suit;
public:
    // Holy four
    Card(RANK rank, SUIT suit);
    ~Card(){}
    Card(const Card& right) { rank = right.rank; suit = right.suit; }
    Card& operator=(const Card& card) { if (this != &card) { /* self copying */ return *this; } return *this; }

    // getters
    RANK getRank() const {return rank;}
    SUIT getSuit() const {return suit;}

    // helper functions
    inline string getSuitString() const
    {
        static const string value_map[] = {"HEARTS", "DIAMONDS", "CLUBS", "SPADES"};
        return value_map[suit];
    }
    inline string getRankString() const
    {
        static const string value_map[] = { "A", "2", "3", "4", "5","6","7","8","9","10", "J", "K", "Q"};
        return value_map[rank];
    }
};

Card::Card(RANK _rank, SUIT _suit)
    : rank(_rank), suit(_suit)
{}

class Deck
{
private:
    std::list<Card> cards;
public:
    Deck();
    ~Deck(){}
    std::list<Card>& getDeck() { return cards; }
    void shuffle();
    // not really purely deck's functions but still...
    static bool isFlush(const std::list<Card>& hand);
    static bool isStraight(const std::list<Card>& hand);
    // helper fuctions
    void printDeck() const;
    static void sort(std::vector<RANK>& card_ranks); // insertion sort
    static std::vector<RANK> extractRanks(const std::list<Card>& from_hand);
    // not sure that we should use isValidHand
    static bool isValidHand(const std::list<Card>& hand);
    static bool compare(std::vector<RANK>& card_ranks, COMPARISON_CRITERIA compasion);
};

bool Deck::compare(std::vector<RANK>& card_ranks, COMPARISON_CRITERIA criteria)
{
    bool result = true;
    for (auto it = card_ranks.begin(); it != card_ranks.end() && result; it++)
    {
        if (next(it) != card_ranks.end())
        {
            auto next_it = next(it);
            if (*next_it - *it != criteria)
            {
                result = false;
            }
        }
    }

    return result;
}

std::vector<RANK> Deck::extractRanks(const std::list<Card>& from_hand)
{
    std::vector<RANK> card_ranks;
    for (const Card& card : from_hand)
    {
        card_ranks.push_back(card.getRank());
    }
    return card_ranks;
}

bool Deck::isValidHand(const std::list<Card> &hand)
{
    std::vector<RANK> card_ranks = extractRanks(hand);

    bool result = true;
    for (auto it = card_ranks.begin(); it != card_ranks.end() && result; it++)
    {
        if (next(it) != card_ranks.end())
        {
            auto next_it = next(it);
            if (*next_it - *it == EQUAL)
            {
                result = false;
            }
        }
    }

    return result; // if there is not EQUAL card_ranks
}

Deck::Deck() {
    cards.clear();
    for (int suitInt = 0; suitInt < SUIT_MAX; suitInt++)
    {
        SUIT suit = static_cast<SUIT>(suitInt);
        for (int rankInt = 0; rankInt < RANK_MAX; rankInt++)
        {
            RANK rank = static_cast<RANK>(rankInt);
            Card card(rank, suit);
            cards.push_back(card);
        }
    }
}

void Deck::sort(std::vector<RANK> &card_ranks)
{
    int i = 1;
    int length = card_ranks.size();
    while (i < length)
    {
        int j = i;
        while (j > 0 && card_ranks[j-1] > card_ranks[j])
        {
            RANK temp = card_ranks[j-1];
            card_ranks[j-1] = card_ranks[j];
            card_ranks[j] = temp;
            j--;
        }
        i++;
    }
}

bool Deck::isStraight(const std::list<Card>& hand)
{
    std::vector<RANK> card_ranks = extractRanks(hand);
    sort(card_ranks);
    return compare(card_ranks, CONSEUQUENT);
}

bool Deck::isFlush(const std::list<Card>& hand)
{
    bool result = true;
    for (std::list<Card>::const_iterator it = hand.begin(); it != hand.end() && result; it++)
    {
        if (next(it) != hand.end())
        {
            std::list<Card>::const_iterator next_it = next(it);
            if (it->getSuit() != next_it->getSuit())
            {
                result = false;
            }
        }
    }

    return result;
}

void Deck::printDeck() const
{
    for (const Card& card : cards)
    {
        cout << card.getRankString() << " " ;
        cout << card.getSuitString() << endl;
    }
}
void Deck::shuffle()
{
    //std::random_device rd;  //Will be used to obtain a seed for the random number engine
    //std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    //std::uniform_int_distribution<> rand_distribution;
    //(0, RANK_MAX * SUIT_MAX - 1);

    for (int i = 0; i < RANK_MAX * SUIT_MAX; i++)
    {
        std::list<Card>::iterator it = cards.begin();
        int r = rand() % (RANK_MAX * SUIT_MAX - 1);
        advance(it, r);
        Card card = *it;
        cards.erase(it);
        cards.push_back(card);
    }

    // For better shuffling
    // rand() lacks or giving low numbers during pseudorandom number generations
    // so we interpret the deck as a "reversed" one, in fact, physically nothing is reversed
    // just swapping bounds of the random distribution
    for (int i = 0; i < RANK_MAX * SUIT_MAX; i++)
    {
        std::list<Card>::iterator it = cards.begin();
        int r = rand() % (RANK_MAX * SUIT_MAX - 1);
        advance(it, 51 - r); // here we interpret the deck in reverse order.
        Card card = *it;
        cards.erase(it);
        cards.push_back(card);
    }
}

int main()
{
    srand(static_cast<unsigned int>(time(nullptr)));
    Deck my_deck;
    my_deck.printDeck();
    my_deck.shuffle();
    my_deck.printDeck();

    const std::list<Card> custom_hand = { {Card(ACE, DIAMONDS)}, {Card(TEN, HEARTS)}, {Card(THREE, HEARTS)}, {Card(FOUR, HEARTS)}, {Card(FIVE, HEARTS)} };
    if (my_deck.isFlush(custom_hand))
    {
        cout << "Flush" << endl;
    }
    else
    {
        cout << "not Flush" << endl;
    }

    if (my_deck.isStraight(custom_hand))
    {
        cout << "Straight" << endl;
    }
    else
    {
        cout << "Not straight" << endl;
    }
    if (my_deck.isValidHand(custom_hand))
    {
        cout << "Valid" << endl;
    }
    else
    {
        cout << "not valid" << endl;
    }

    return 0;
}
