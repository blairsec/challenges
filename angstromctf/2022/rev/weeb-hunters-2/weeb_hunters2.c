#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>



char* forestMessages[] = {
	"A soft light emanates from above. All around you are trees, several times your height.",
	"You hear footsteps nearby, but you can't quite make out where they're coming from.",
	"A foreboding feeling washes over you. \"This can't be good,\" you think.",
	"You hear foreign babbling from all directions, sending chills down your spine.",
	"The forest has turned dark. You can barely make out your surroundings.",
};

char *moveMessages[] = {
	"The twisting passages of the forest make you wonder if you went the right way.",
	"You appear to be going the right way, but you lose track of your position.",
	"You glance down at your map. \"Where am I?\" you wonder.",
	"The warped geometry of the forest makes navigation difficult.",
	"The darkness of the forest causes you to lose your way, and you're not sure where you are."
};
int haveSword = 0;
int haveWhistle = 0;
int haveNewspaper = 0;
int haveFist = 0;
char *weebNames[] = {
	"joshdabosh",
	"evilmuffinha",
	"pertark"
};
#define NUM_COMMANDS 8
int commands[NUM_COMMANDS][32] = {
	{236115, 622159, 87468, 680108, 321855, 0}, // sword
	{17595, 147807, 295934, 38075, 472094, 68039, 112609, 0}, // whistle
	{262412, 117919, 622331, 226699, 30675, 249530, 178623, 116649, 685375, 0}, // newspaper
	{40647, 20406, 221523, 169081, 216857, 497828, 170295, 104344, 173849, 91086, 112967, 53726, 85161, 155307, 61063, 41845, 0}, // sword activation
	{90962, 91012, 91064, 170606, 154586, 243948, 83044, 106725, 60679, 249545, 51762, 162681, 81319, 160927, 252216, 108431, 0}, // newspaper activation
	{253251, 51627, 28191, 332180, 244338, 113151, 60673, 30811, 117247, 45947, 58011, 91177, 8161, 87487, 326505, 332128, 0}, // whistle activation
	{224194, 295630, 40219, 467999, 0}, // fist
	{221257, 382942, 116844, 116647, 0} // flee
};
int main() {
	printf("Welcome to the new Weeb Hunters 2: The Flag of Power!\n");
	printf("Enter your name: ");
	char name[40];
	scanf("%39s", name);
	printf("You are %s, a valiant weeb hunter. You will be traversing the dangerous Otaku Forest, where weebs surround you in every direction.\n", name);
	printf("Along the way, you will find many friends and foes on your quest to destroy the weebs once and for all.\n");
	printf("You will collect magical artifacts, but you can only use them if you find out the right command phrase!\n");
	int randSeed = time(NULL);
	char keystream[33*4] = "weebhunters2";
	for (int i = 1; i < 11; i++) {
		strncpy(keystream+(i*12), keystream, 12);
	}
	for (int i = 0; i < 32; i++) {
		randSeed >>= 1;
		randSeed ^= keystream[i*4]; // shuffle up the time a bit
	}
	srand(randSeed); // real random generator
	short playerX = 0xdead;
	short playerY = 0xbeef;
	char action[40];
	int sanity = 100;
	int inCombat = 0;
	char opponent[40];
	int animeDays;
	int finalBoss = 0;
	while (1) {
	main_loop:
		if (inCombat) {
			if (inCombat == 1) {
				strcpy(opponent, weebNames[(playerX*playerY) % 3]);
				printf("You hear a noise behind you... \"BAKA!!!\" %s comes charging at you.\n", opponent);
				animeDays = *(int*)opponent % 100;
				inCombat = 2;
			}
			printf("Choose your action: ");
			scanf("%39s", action);
			// actions:
			// sword - deals 8 damage
			// whistle - deals 6 damage
			// newspaper - deals 1 damage
			// "54fcfbc921e8b735" - uses upgraded sword
			// "111c7ab93a8cbcae" - uses upgraded newspaper
			// "a8cdae3240310bdd" - uses upgraded whistle
			// fist - uses fist, if you have it (no damage)
			// flee - tries to flee (drops all your weapons)
			char decaction[40];
			for (int i = 0; i < NUM_COMMANDS; i++) {
				int j;
				for (j = 0; commands[i][j]; j++) {
					int k = 0;
					int value = commands[i][j];
					while (value != 1) {
						if (value % 2 == 0) {
							value >>= 1;
						} else {
							value = 3*value + 1;
						}
						k++;
					}
					decaction[j] = k - 128;
				}
				decaction[j] = 0;
				if (!strcmp(action, decaction)) {
					switch(i) {
						case 0:
							if (!haveSword) goto combat_death1;
							printf("You slash the weeb's arm off. \"'tis but a scratch,\" he claims, but it's obviously a little worse than that. In the intense pain, he has forgotten 8 days worth of anime.\n");
							animeDays -= 8;
							break;
						case 1:
							if (!haveWhistle) goto combat_death1;
							printf("You blow into the whistle and the piercing sound engulfs the weeb's sensory system. He forgets 6 days worth of anime.\n");
							animeDays -= 6;
							break;
						case 2:
							if (!haveNewspaper) goto combat_death1;
							printf("You bop the weeb on the head with the rolled up newspaper. He suffers minor brain damage and forgets a single day of anime.\n");
							animeDays -= 1;
							break;
						case 3:
							if (haveSword != 2) goto combat_death1;
							printf("You slash the weeb's arm off. \"'tis but a scratch,\" he claims, but it's obviously a little worse than that. In the intense pain, he has forgotten 100 days worth of anime.\n");
							animeDays -= 100;
							if (finalBoss) {
								printf("The boss retaliates quickly and disarms you with a sweeping blow of anime. You've lost your sword.\n");
								haveSword = 0;
							}
							break;
						case 4:
							if (haveWhistle != 2) goto combat_death1;
							printf("You blow into the whistle and the piercing sound engulfs the weeb's sensory system. He forgets 100 days worth of anime.\n\n");
							animeDays -= 100;
							if (finalBoss) {
								printf("The boss retaliates quickly and disarms you with a sweeping blow of anime. You've lost your whistle.\n");
								haveWhistle = 0;
							}
							break;
						case 5:
							if (haveNewspaper != 2) goto combat_death1;
							printf("You bop the weeb on the head with the rolled up newspaper. He suffers minor brain damage and forgets 100 days of anime.\n");
							animeDays -= 100;
							if (finalBoss) {
								printf("The boss retaliates quickly and disarms you with a sweeping blow of anime. You've lost your newspaper.\n");
								haveNewspaper = 0;
							}
							break;
						case 6:
							if (haveFist == 0) {
								printf("You try to hit the weeb with your fist, but then you realize that you left it behind in the last battle!\n");
							} else if (haveFist == 1) {
								printf("Your fist is still not attached to your arm yet!\n");
							} else {
								if (finalBoss && (haveWhistle + haveNewspaper + haveSword == 0) && animeDays <= 100) {
									printf("You hit the weeb with your fist, vanquishing it forever into the void. You have restored peace to this land and destroyed forever the reign of the weebs.\n");
									printf("You also got the flag, it's actf{sorry_guys_no_double_free_this_time}!\n");
									return 0;
								}
								printf("You hit the weeb with your fist, but you don't deal any damage.\n");
								break;
							}
							goto combat_death;
						case 7:
							printf("Knowing you have no chance of defeating someone who's watched %d days of anime, you run away as fast as you can. In your haste, you leave your weapons behind.\n", animeDays);
							inCombat = 0;
							haveSword = 0;
							haveNewspaper = 0;
							haveWhistle = 0;
							if (haveFist < 2) haveFist = 0;
							goto main_loop;
						default:
							printf("Unimplemented action\n");
					}
				}
			}
			if (animeDays <= 0) {
				printf("%s stiffens as he realizes that he doesn't remember any anime anymore. You have vanquished a weeb!\n", opponent);
				if (!haveFist) {
					printf("You search %s's possessions and find your fist that you left behind.\n", opponent);
					haveFist = 1;
				}
				inCombat = 0;
				continue;
			}
			printf("%s emits a piercing scream. \"Explosion\"! You are engulfed in a fireball as the sounds of anime begin to close in...\n", opponent);
			sanity -= 20;
			if (sanity <= 0) {
				goto combat_death;
				combat_death1:
				printf("You grasp for your weapon, but you can't find anything.\n");
				combat_death:
				printf("%s begins watching anime in front of you and you become entranced. Otaku, kawaii, senpai... you have become one of them.\n", opponent);
				return 0;
			}
			continue;
		}
		printf("%s\n", forestMessages[rand() % 5]);
		if (sanity == 100) {
			printf("Your mind is clear of any distractions, and you are prepared for your quest.\n");
		} else if (sanity > 75) {
			printf("A lingering doubt is at the back of your mind. You press onwards.\n");
		} else if (sanity > 50) {
			printf("You're starting to wonder whether you can complete your mission.\n");
		} else if (sanity > 25) {
			printf("Your ears ring with the sounds of anime, and it's hard to focus.\n");
		} else if (sanity > 0) {
			printf("It's hard to keep your mind off of anime, and you fear you have almost become a weeb yourself.\n");
		} else {
			printf("You fall to the ground in exhaustion. Weebs surround you, but you are helpless to defend yourself.\n");
			strcpy(opponent, weebNames[(playerX*playerY) % 1]);
			goto combat_death;
		}
		printf("Which way would you like to go?\n> ");
		scanf("%39s", action);
		// movement code, shuffle position randomly
		for (int i = 0; i < strlen(action); i++) {
			if (action[i] < ' ' || action[i] >= 127) {
				printf("Babbling incoherently, you try to invoke the dark art known as \"pwn\", but your will isn't strong enough.\n");
				sanity = 0;
			}
		}
		if (playerX == 0 && playerY == 0 && !strcmp(action, "a12caf3cba1e5f13")) {
			printf("You utter the incantation and the ground shakes. A set of stairs lead down into the darkness...\n");
			printf("You walk into the dungeon, and you see a single figure standing before you! As you prepare to face your opponent, you know that this is the final battle!\n");
			strcpy(opponent, "kmh");
			inCombat = 2;
			finalBoss = 1;
			animeDays = 400;
			continue;
		}
		srand(rand() ^ *(int*)action); // mix in new randomness
		short moveX = rand();
		short moveY = rand();
		if (moveX > 0) playerX += moveX;
		if (moveY > 0) playerY += moveY;
		printf("%s\n", moveMessages[rand() % 5]);
		// world gen code
		short seed1, seed2, seed3, seed4;
		seed1 = playerX ^ playerY;
		seed2 = (playerX & playerY) + (playerX | playerY);
		seed3 = playerX * playerY;
		seed4 = seed1+seed2+seed3;
		long location = ((unsigned long)seed1) | ((unsigned long)seed2 << 16) | ((unsigned long)seed3 << 32) | ((unsigned long)seed4 << 48);
		switch(location) {
			case 0x33c458487662651a:
				printf("You find a blacksmith's house in the depths of the forest. \"For years, I have waited for somebody to defeat the weebs that haunt this forest,\" he says.\n");
				if (haveSword) {
					printf("\"I can improve your sword and make it sharper. Good luck!\" You've been equipped with a powerful weapon.\n");
					haveSword = 2;
				} else {
					printf("\"Bring me a sword, and I can make it more deadly towards the weebs.\" You don't have any swords on you.\n");
				}
				break;
			case 0x5b3e7b48765369a3:
				printf("You find a wizard's tower in the depths of the forest. \"For years, I have waited for somebody to defeat the weebs that haunt this forest,\" he says.\n");
				if (haveNewspaper) {
					printf("\"I can enchant your newspaper and cause it to hurt the weebs. Good luck!\" You've been equipped with a powerful weapon.\n");
					haveNewspaper = 2;
				} else {
					printf("\"Bring me a newspaper, and I can give it mystical powers.\" You don't have any newspapers on you.\n");
				}
				break;
			case 0xdbe762ed608e186c:
				printf("You find a engineer's shack in the depths of the forest. \"For years, I have waited for somebody to defeat the weebs that haunt this forest,\" he says.\n");
				if (haveWhistle) {
					printf("\"I can improve your whistle and make it more damaging to the weebs. Good luck!\" You've been equipped with a powerful weapon.\n");
					haveWhistle = 2;
				} else {
					printf("\"Bring me a whistle and I can make its sound more painful.\" You don't have any whistles on you.\n");
				}
				break;
			case 0xd3385c606d740964:
				printf("You find a doctor's hut in the depths of the forest. \"For years, I have waited for somebody to defeat the weebs that haunt this forest,\" he says.\n");
				if (haveFist == 1) {
					printf("\"I can reattach your fist to your arm. Good luck!\" Your fist is back on!\n");
					haveFist = 2;
				} else if (haveFist == 2) {
					printf("\"I have already done all I can do for your mission. Good luck!\"\n");
				} else {
					printf("You remember that you left your fist behind in your previous quest, and ask the doctor if he can help. \"If you bring me your fist, I can reattach it to your arm,\" he says.\n");
				}
				break;
			default:
				if (location % 8 == 0) inCombat = 1;
				else {
					if (location % 8 == 1) {
						switch((location >> 3) & 3) {
							case 0:
								printf("You found a sword!\n");
								if (haveSword == 0) haveSword = 1;
								break;
							case 1:
								printf("You found a newspaper!\n");
								if (haveNewspaper == 0) haveNewspaper = 1;
								break;
							case 2:
								printf("You found a whistle!\n");
								if (haveWhistle == 0) haveWhistle = 1;
								break;
							default:
								break;
						}
					}
				}
		}
		sanity--;
	}
}
