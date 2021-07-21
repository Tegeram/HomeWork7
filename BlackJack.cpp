#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Card {
	
public:
	enum rank { ACE = 1, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK = 10, QUEEN = 10, KING = 10 };
	enum suit { CLUBS, DIAMONDS, HEARTS, SPADES };

	friend ostream& operator<<(ostream& os, const Card& c);

	Card(rank r = ACE, suit s = SPADES, bool ifu = true);

	int GetValue() const;

	void Flip();

	rank GetRank() const {
		return m_Rank;
	};

	bool GetIsFace() const {
		return m_IsFaceUp;
	}

protected:
	rank m_Rank;
	suit m_Suit;
	bool m_IsFaceUp;
};

Card::Card(rank r, suit s, bool ifu) : m_Rank(r), m_Suit(s), m_IsFaceUp(ifu) {}

int Card::GetValue() const {
	int value = 0;
	if (m_IsFaceUp) value = m_Rank;
	return value;
}

void Card::Flip() {
	m_IsFaceUp = !(m_IsFaceUp);
}

ostream& operator<< (ostream & os, const Card & c) {
	const string Rank[] = {"0", "A", "2", "3", "4", "5", "6", "7", "8", "9","10", "J", "Q", "K" };
	const string Suit[] = { "c", "d", "h", "s" };
	if (c.m_IsFaceUp) os << Rank[c.m_Rank] << Suit[c.m_Suit] << " ";
	else os << "XX ";

	return os;
}

//==================================================================================================

class Hand {
protected:
	vector<Card*> HandCards;
public:
	Hand() {
		HandCards.reserve(10);
	}

	~Hand() { Clear(); }

	void Add(Card* c) {
		HandCards.push_back(c);
	}

	void Clear() {
		HandCards.clear();
	}

	int GetValue() const {
		int value = 0;
		bool haveace = false;
		bool addten = false;
		for (size_t i = 0; i < HandCards.size(); i++)
		{		
			if (HandCards[i]->GetRank() == Card::ACE) {
				haveace = true;
				if (HandCards[i]->GetIsFace()) addten = true;
			}
			value += HandCards[i]->GetValue();
		}		
		if (haveace && addten && value <= 11) value += 10;
		return value;
	}	
};

//==================================================================================================

class GenericPlayer : public Hand {
protected:
	string Name;
public:
	GenericPlayer(string name = " "): Name(name) {}

	~GenericPlayer() {};

	virtual bool IsHitting() const = 0;

	bool IsBusted() const {
		return (GetValue() > 21);
	}

	void Bust() const {
		cout << " " << Name << " - bust" << endl;
	}

	friend ostream& operator<<(ostream& os, const GenericPlayer& gp);
	
};

ostream& operator<<(ostream& os, const GenericPlayer& gp) {
	os << gp.Name << ":\t";

	vector<Card*>::const_iterator c;
	if (!gp.HandCards.empty()) {
		for (c = gp.HandCards.begin(); c != gp.HandCards.end(); ++c) {
			os << *(*c);
		}
		if (gp.GetValue() != 0) cout << gp.GetValue() << " points";
	}
	else os << "empty";

	return os;
}

//==================================================================================================

class Player : public GenericPlayer {
public:
	Player(string name) : GenericPlayer(name){}
	~Player(){}
	bool IsHitting() const {
		
		cout << "\n" << Name << ", do you want a hit? (Yes/No): ";
		char response;
		cin >> response;
		return (response == 'Y' || response == 'y' || response == 'yes' || response == 'Yes');
	}

	void Win() const {
		cout << " " << Name << " - win!" << endl;
	}

	void Lose() const {
		cout << " " << Name << " - lose!" << endl;
	}

	void Push() const {
		cout << " " << Name << " - played in draw!" << endl;
	}
};

//==================================================================================================

class House : public GenericPlayer {
public:
	House(string name = "House") : GenericPlayer(name){}
	~House(){}
	bool IsHitting() const {
		return (GetValue() <= 16);
	}

	void FlipFirstCard() {
		if (!(HandCards.empty())) HandCards[0]->Flip();
		else cout << "No card to flip!" << endl;
	}
};

//==================================================================================================

class Deck : public Hand {
public:
	Deck() {
		HandCards.reserve(52);
		Populate();
	}

	virtual ~Deck() {};

	void Populate() {
		Clear();

		for (int s = Card::CLUBS; s <= Card::SPADES; ++s) {
			for (int r = Card::ACE; r <= Card::KING; ++r) {
				Add(new Card(static_cast<Card::rank>(r), static_cast<Card::suit>(s)));
			}
		}
	}
	void Shuffle() {
		random_shuffle(HandCards.begin(), HandCards.end());
	}

	void Deal(Hand& aHand) {
		if (!HandCards.empty())
		{
			aHand.Add(HandCards.back());
			HandCards.pop_back();
		}
		else cout << "Out of cards. Unable to deal.";
	}

	void AdditionalCards(GenericPlayer& aGP) {
		while (!(aGP.IsBusted()) && aGP.IsHitting()) {
			Deal(aGP);
			cout << aGP << endl;
			if (aGP.IsBusted()) aGP.Bust();
		}
	}
};

//==================================================================================================

class Game {
protected:
	Deck m_deck;
	vector<Player> m_players;
	House m_house;
public:
	Game(const vector<string>& names) {
		vector<string>::const_iterator pName;
		for (pName = names.begin(); pName != names.end(); ++pName) {
			m_players.push_back(Player(*pName));
		}

		srand(static_cast<unsigned int>(time(0)));
		m_deck.Populate();
		m_deck.Shuffle();
	}

	~Game() {

	}

	void play() {
		vector<Player>::iterator pPlayer;
		for (int i = 0;i < 2;++i) {
			for (pPlayer = m_players.begin();pPlayer != m_players.end();++pPlayer) {
				m_deck.Deal(*pPlayer);
			}
			m_deck.Deal(m_house);
		}

		m_house.FlipFirstCard();

		for (pPlayer = m_players.begin(); pPlayer != m_players.end(); ++pPlayer) {
			cout << *pPlayer << endl;
		}
		cout << m_house << endl;

		for (pPlayer = m_players.begin(); pPlayer != m_players.end(); ++pPlayer) {
			m_deck.AdditionalCards(*pPlayer);
		}

		m_house.FlipFirstCard();
		cout << endl << m_house << endl;

		m_deck.AdditionalCards(m_house);

		if (m_house.IsBusted()) {			
			for (pPlayer = m_players.begin(); pPlayer != m_players.end(); ++pPlayer) {
				if (!(pPlayer->IsBusted())) {
					pPlayer->Win();
				}
			}				
		}
		else {
			for (pPlayer = m_players.begin(); pPlayer != m_players.end(); ++pPlayer) {
				if (!(pPlayer->IsBusted())) {
					if (pPlayer->GetValue() > m_house.GetValue()) {
						pPlayer->Win();
					}
					else if (pPlayer->GetValue() < m_house.GetValue()) {
						pPlayer->Lose();
					}
					else pPlayer->Push();
				}
			}
		}

		for (pPlayer = m_players.begin(); pPlayer != m_players.end(); ++pPlayer) {
			pPlayer->Clear();
		}
		m_house.Clear();

		if (Again()) {
			system("cls");
			play();
		}
	}

	bool Again() {
		cout << "\n"  << "Want play again? (Yes/No): ";
		char response;
		cin >> response;
		return (response == 'Y' || response == 'y' || response == 'yes' || response == 'Yes');
	}
};

//==================================================================================================

int main()
{
	int n;
	cout << "How many players in game: ";
	cin >> n;
	string name;
	vector<string> plv;
	for (size_t i = 1; i <= n; i++)	{
		cout << "Enter " << i << " name: ";
		cin >> name;
		plv.push_back(name);		
	} 
	Game game(plv);
	game.play();
	return 0;
}
