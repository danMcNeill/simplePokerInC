#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "poker.h"

/* Global instances of the two players */
struct player P1, P2;

/* Parse card from a 2 char input */
struct card parse(const char *card)
{
	/* TASK 14: parse input and store in card data structure. */
	struct card c;
	/* Hint: You could use switch statements here */

	switch(*card) {
		case '2': c.val = c2;
			break;
		case '3': c.val = c3;
			break;
		case '4': c.val = c4;
			break;
		case '5': c.val = c5;
			break;
		case '6': c.val = c6;
			break;
		case '7': c.val = c7;
			break;
		case '8': c.val = c8;
			break;
		case '9': c.val = c9;
			break;
		case 'T': c.val = c10;
			break;
		case 'J': c.val = JACK;
			break;
		case 'Q': c.val = QUEEN;
			break;
		case 'K': c.val = KING;
			break;
		case 'A': c.val = ACE;
			break;
	}

	card++;

	switch(*card) {
		case 'S': c.suit = SPADES;
			break;
		case 'H': c.suit = HEARTS;
			break;
		case 'D': c.suit = DIAMONDS;
			break;
		case 'C': c.suit = CLUBS;
			break;
	}
	
	return c;
}

/* Count the number of occurrences of each card 2 through Ace */
void count_cards(struct player *p)
{
	/* TASK 13:  
	 * Iterate through the cards and increment the counter for each card
	 */
	int i = 0;
	int cardValue;
	for(i=0; i<5; i++) {
		cardValue = p->cards[i].val;
		if(cardValue == 2)
			p->card_count[0]++;
		else if(cardValue == 3)
			p->card_count[1]++;
		else if(cardValue == 4)
			p->card_count[2]++;
		else if(cardValue == 5)
			p->card_count[3]++;
		else if(cardValue == 6)
			p->card_count[4]++;
		else if(cardValue == 7)
			p->card_count[5]++;
		else if(cardValue == 8)
			p->card_count[6]++;
		else if(cardValue == 9)
			p->card_count[7]++;
		else if(cardValue == 10)
			p->card_count[8]++;
		else if(cardValue == JACK)
			p->card_count[9]++;
		else if(cardValue == QUEEN)
			p->card_count[10]++;
		else if(cardValue == KING)
			p->card_count[11]++;
		else if(cardValue == ACE)
			p->card_count[12]++;
	}
}

int is_flush(struct player *p)
{
	/* TASK 12:
	 * Hand is flush if suit of all the cards is the same
	 */

	int s = (*p).cards[0].suit;
	if((s == (*p).cards[1].suit) && (s == (*p).cards[2].suit) && (s == (*p).cards[3].suit) && (s == (*p).cards[4].suit))
		return 1;
	
	return 0; /* Return 1 if flush */
}

int is_straight(struct player *p)
{
	/* NOTE: By this stage, the vector must be constructed.
	 * 
	 * The hand is a straight, if the cards are sequential.
	 * A2345 as well as TJQKA are both valid straights, and Ace 
	 * assumes the value of 1 or 13, but not both.
	 */

	/* TASK 11: Check for regular straights
 	 * Hint: If this is a straight, player's card_count for i, i+1, i+2, i+3 and i+4 should be 1. */
	
	int i = 0;

	for(i=0; i<9; i++) {
		if((p->card_count[i] == 1) && (p->card_count[i+1] == 1) && (p->card_count[i+2] == 1) && (p->card_count[i+3] == 1) && (p->card_count[i+4] == 1))
			return 1;
	}

	/* Check for A2345
	 * TASK 10: If the hand is A2345, reset the bit for highcard Ace in the player's vector to appropriately reflect that the value of A is not 13. 
	 * Hint: 1 << position will generate a bit mask for the position of Ace. Xoring it with the vector will reset the bit.
	 */

	if(((*p).card_count[0] == 1) && ((*p).card_count[1] == 1) && ((*p).card_count[2] == 1) && ((*p).card_count[3] == 1) && ((*p).card_count[12] == 1)) {
		p->vector ^= 1 << 12;
		return 1;
	}

	return 0; /* Return 1 if straight */	
}

int is_straight_flush(struct player *p)
{
	/* TASK 9: Detect straight flush. A straight flush is nothing but a straight and a flush */
	
	int flush = is_flush(p);
	int straight = is_straight(p);
	if((flush == 1) && (straight == 1))
		return 1;
	return 0; /* Return 1 if straight flush */
}

int is_full_house(struct player *p) { 

	int x = 0;
	int numPairs = 0;
	int numTrips = 0;
	for(x=0; x<13; x++) {
		if((*p).card_count[x] == 2) {
			numPairs++;
		}
		else if((*p).card_count[x] == 3) {
			numTrips++;
		}
	}
	if((numPairs == 1) && (numTrips == 1)) {
		return 1;
	}
	else
		return 0;

	

}
		
/* This is the main function that converts the player's hand into weighted unsigned long number. 
It is a 56bit vector as shown below (2 is the LSB and StraightFlush is the MSB) */
/* 2, 3, 4, 5, 6, 7, 8, 9, T, J, Q, K, A, 22, 33, 44, 55, 66, 77, 88, 99, TT, JJ, QQ, KK, AA,
222, 333, 444, 555, 666, 777, 888, 999, TTT, JJJ, QQQ, KKK, AAA, Straight, Flush, FullHouse, 2222, 3333, 
4444, 5555, 6666, 7777, 8888, 9999, TTTT, JJJJ, QQQQ, KKKK, AAAA, StraightFlush */
/* The number of occurrences of each number in the hand is first calculated in count_cards. 
Then, depending on the numeber of occurrences, the corresponding bit in the bit-vector is set. 
In order to find the winner, a simple comparison of the bit vectors (i.e., unsigned long integers) will suffice! */
void eval_strength(struct player *p)
{
	unsigned int straight_or_flush_possible;
	unsigned int i;
	straight_or_flush_possible = 1; 

	/*
	 * TASK 6: Invoke the function to count the occurrences of each card
	 */
	count_cards(p);

	/*
	 * TASK 7: Iterate through the card_count array, and set the appropriate bit in the bit vector.
	 * For example, if the hand is JH JS 3C AH 4H, set the 23rd bit to indicate a pair of jacks, and 2nd, 3rd and 13th bit to indicate 3, 4 and A respectively. */

	
	for(i=0; i<13; i++) {
		switch(p->card_count[i]) {
			case 1 : p->vector |= 1UL << (i);
				break;
			case 2 : p->vector |= 1UL << (13+i);
				break;
			case 3 : p->vector |= 1UL << (26+i);
				break;
			case 4 : p->vector |= 1UL << (42+i);
				break;
		}
	}



/*

switch(*p.card_count[0]) {  //twos
		case 1 : *p.vector |= 1 << 1;
			break;
		case 2 : *p.vector |= 1 << 14;
			break;
		case 3 : *p.vector |= 1 << 27;
			break;
		case 4 : *p.vector |= 1 << 43;
			break;
	}

	switch(*p.card_count[1]) {  //threes
		case 1 : *p.vector |= 1 << 2;
			break;
		case 2 : *p.vector |= 1 << 15;
			break;
		case 3 : *p.vector |= 1 << 28;
			break;
		case 4 : *p.vector |= 1 << 44;
			break;
	}

	switch(*p.card_count[2]) {  //fours
		case 1 : *p.vector |= 1 << 3;
			break;
		case 2 : *p.vector |= 1 << 16;
			break;
		case 3 : *p.vector |= 1 << 29;
			break;
		case 4 : *p.vector |= 1 << 45;
			break;
	}

	switch(*p.card_count[3]) {  //fives
		case 1 : *p.vector |= 1 << 4;
			break;
		case 2 : *p.vector |= 1 << 17;
			break;
		case 3 : *p.vector |= 1 << 30;
			break;
		case 4 : *p.vector |= 1 << 46;
			break;
	}

	switch(*p.card_count[4]) {  //six
		case 1 : *p.vector |= 1 << 5;
			break;
		case 2 : *p.vector |= 1 << 18;
			break;
		case 3 : *p.vector |= 1 << 31;
			break;
		case 4 : *p.vector |= 1 << 47;
			break;
	}

	switch(*p.card_count[5]) {  //seven
		case 1 : *p.vector |= 1 << 6;
			break;
		case 2 : *p.vector |= 1 << 19;
			break;
		case 3 : *p.vector |= 1 << 32;
			break;
		case 4 : *p.vector |= 1 << 48;
			break;
	}

	switch(*p.card_count[6]) {  //eight
		case 1 : *p.vector |= 1 << 7;
			break;
		case 2 : *p.vector |= 1 << 20;
			break;
		case 3 : *p.vector |= 1 << 33;
			break;
		case 4 : *p.vector |= 1 << 49;
			break;
	}

	switch(*p.card_count[7]) {  //nine
		case 1 : *p.vector |= 1 << 8;
			break;
		case 2 : *p.vector |= 1 << 21;
			break;
		case 3 : *p.vector |= 1 << 34;
			break;
		case 4 : *p.vector |= 1 << 50;
			break;
	}

	switch(*p.card_count[8]) {  //ten
		case 1 : *p.vector |= 1 << 9;
			break;
		case 2 : *p.vector |= 1 << 22;
			break;
		case 3 : *p.vector |= 1 << 35;
			break;
		case 4 : *p.vector |= 1 << 51;
			break;
	}

	switch(*p.card_count[9]) {  //jack
		case 1 : *p.vector |= 1 << 10;
			break;
		case 2 : *p.vector |= 1 << 23;
			break;
		case 3 : *p.vector |= 1 << 36;
			break;
		case 4 : *p.vector |= 1 << 52;
			break;
	}

	switch(*p.card_count[10]) {  //queen
		case 1 : *p.vector |= 1 << 11;
			break;
		case 2 : *p.vector |= 1 << 24;
			break;
		case 3 : *p.vector |= 1 << 37;
			break;
		case 4 : *p.vector |= 1 << 53;
			break;
	}

	switch(*p.card_count[11]) {  //king
		case 1 : *p.vector |= 1 << 12;
			break;
		case 2 : *p.vector |= 1 << 25;
			break;
		case 3 : *p.vector |= 1 << 38;
			break;
		case 4 : *p.vector |= 1 << 54;
			break;
	}

	switch(*p.card_count[12]) {  //ace
		case 1 : *p.vector |= 1 << 13;
			break;
		case 2 : *p.vector |= 1 << 26;
			break;
		case 3 : *p.vector |= 1 << 39;
			break;
		case 4 : *p.vector |= 1 << 55;
			break;
	} */

	/* TASK 8:
	 * Check if this is a straight, flush or a straight flush, and set the appropriate bit in the vector.
	 */

	int flush = is_flush(p);
	int straight = is_straight(p);
	int straightFlush = is_straight_flush(p);
	int fullHouse = is_full_house(p);

	if(fullHouse == 1)
		p->vector |= 1UL << 41;

	if(straightFlush == 1)
		p->vector |= 1UL << 55;
	else if(flush == 1)
		p->vector |= 1UL << 40;
	else if(straight == 1)
		p->vector |= 1UL << 39;

}

/* Parse each hand in the input file, evaluate the strengths of hands and identify a winner by comparing the weighted vectors */
void compare_hands(FILE *fp)
{
	char p1[5][3];
	char p2[5][3];
	int i;

	while(fscanf(fp, "%s %s %s %s %s %s %s %s %s %s", 
		&p1[0][0], &p1[1][0], &p1[2][0], &p1[3][0], &p1[4][0], 
		&p2[0][0], &p2[1][0], &p2[2][0], &p2[3][0], &p2[4][0]) == 10) {
		memset(&P1, 0, sizeof(struct player));
		memset(&P2, 0, sizeof(struct player));
		for(i = 0; i < 5; i++) {
			P1.cards[i] = parse(&p1[i][0]);
			P2.cards[i] = parse(&p2[i][0]);
		}
		
		/* TASK 4: Invoke eval_strength for each player */
		eval_strength(&P1);
		eval_strength(&P2);
		/* TASK 5: Report the winner (e.g., "Player 1 wins") depending on whoever has the higher vector */
		if(P1.vector > P2.vector) {
			FILE *ofp;
			ofp = fopen("Output.txt", "a");
			fprintf(ofp, "Player 1 wins\n");
			fclose(ofp);
		}
		else {
			FILE *ofp;
			ofp = fopen("Output.txt", "a");
			fprintf(ofp, "Player 2 wins\n");
			fclose(ofp);
		}

	}
	fclose(fp);
}

int main(int argc, char *argv[])
{
	FILE *fp;

	/* TASK 2: Validate command line arguments and try to open the file. Exit on failure. */
	
	fp = fopen(argv[1], "r");
	if(fp == 0) {
		printf("Could not open file. Exiting.\n");
		return 0;
	}

	/* TASK 3: Invoke the compare hands function */

	else {
		compare_hands(fp);
	}

	return 0;
}
	
