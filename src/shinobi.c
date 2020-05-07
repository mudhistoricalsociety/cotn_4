#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "interp.h"


/* Shinobi Class by Kasai for use on DarkSkies mud.
	Fire, Wind, Earth, Water, Shadow, Holy

			Fire
		Earth		Shadow

		Holy		Wind
			Water

	Learn Master of your element
	Learn Medium of closest 2
	Learn Basics of next 2
	Fire Shin are good melee, but mainly special attack fighters. Shadow are all melee, Earth is mostly melee, Wind is a balance,
	 Water is melee, Holy is Special

	Hand Motions:
		Wraith, Monkey, Serpent, Spider, Wolf, Dragon, Lion, Rabbit, Boar, Shark, Eagle, Demon,
		Angel, Souju.
		Each motion can be learned up to 10, you can use 'handmotion' to learn them.

		You must meditate to obtain chakra.
		Reach chakra Max to go to next level of ninja.
		Level	Max Chakra
		1	100
		2	200
		3	350 - Medium Level
		4	500
		5	800
		6	1000- Master Level
		7	1500
		8	2000
		9	3000
		10	5000 - Kage Level
	Jutsus:
		Shadow- Sneaky Shit
			Vensai - Travel through the shadows.
			Butomi - Create illusions of yourself. 1 + Status / 10 clones.
			Sami - Become Invisible.
			Sau Tchen - Shadow Chains, deal damage and web.
			Daemi - Infuse your soul with demon essence.
			Sau Butomi - Your illusions can dodge/parry/damage.

		Fire- Brute Force
			Fagra - Shoot a small bolt of flame. Damage based on rank.
			Torra - Create a wall of caltrops.
			Bae - Blind your target with fire.
			Taiegra - Create a flaming shield that acts like eye for an eye.
			Ennotate - Powerful fire damage.
			Lava - Fill the room with deadly lava, causing high damage each round
			to all players in the room who are not shinobi (barring water).

		Earth- Powerful Defense
			Eagra - Shield
			Inos - Swallow any target with the earth and teleport them.
			Yaoe - Slow down your opponent in battle. Counts as web.
			Easpi - Earthen attack.
			Eshina - Become one with the earth. Higher Stats + Immune to vorpal.
			Sha - Automatic ability. Harden every pulse out of battle, weaken in battle
			
		Wind- Bizzare shit.
			Updraft - Travel command.
			Aero - Area attack.
			Hoana - Scry
			Chagi - Player affected by this sees players randomly.
			Aeblade - Powerful wind attack.
			Tornado - Surround the room with a swirling vortex.
			Wind Shinobis are invisible in this during combat and
			gain something like drow fightdance. Harms earth shinobi.
			Can not summon with Earth Shinobi in the room.

		Water- Healing Restriction-
			Soothe - Healing power.
			Wash - Freeze your enemy, they attack slower.
			Fluidity - Enhanced dodging powers + attacks per turn.
			Aqua - Water blasting attack.
			ViuNemTo - Turn into a puddle, specify a player. Ambush when they enter.
			Storm - Increase the pk powers of water Shinobi in the room. Damage Fire Shin
			and other players in the room.

		Holy - Healing/Special Attacks
			Shoju - Increase the average damage of a weapon. Anti-evil it.
			Mian - Heal hp-mana-move+web/blindfold/tie.
			GoaZenTo - No one can fight in the room you are in, you lose hp violenceupdate
				doing this.
			Hishu - Holy Attack
			Beouto - For every member you have in your group who is a shinobi,
				your bless gets an additional 500 hr/dr -750 ac. All group members get 	this.
			Yu - Fill the room with holy radiance, damage all evil aligned players each turn.
	

*/

#define JFAIL	{stc("Who do you wish to target with this?\n\r",ch);return;}

void jutsu_fagra args ((CHAR_DATA *ch, CHAR_DATA *tarchar));
void jutsu_torra args ((CHAR_DATA *ch, int dir));
void jutsu_bae args ((CHAR_DATA *ch, CHAR_DATA *tarchar));
void jutsu_taiegra args ((CHAR_DATA *ch));
void jutsu_ennotate args ((CHAR_DATA *ch, CHAR_DATA *tarchar));
void jutsu_lava args ((CHAR_DATA *ch));
void jutsu_vensai args ((CHAR_DATA *ch, CHAR_DATA *victim));
void jutsu_butomi args ((CHAR_DATA *ch));
void jutsu_sautchen args ((CHAR_DATA *ch, CHAR_DATA *tarchar));
void jutsu_sami args ((CHAR_DATA *ch));
void jutsu_daemi args ((CHAR_DATA *ch));
void jutsu_updraft args ((CHAR_DATA *ch, CHAR_DATA *victim));
void jutsu_aero args ((CHAR_DATA *ch));
void jutsu_hoana args ((CHAR_DATA *ch, CHAR_DATA *victim));
void jutsu_chagi args ((CHAR_DATA *ch, CHAR_DATA *victim));
void jutsu_aeblade args ((CHAR_DATA *ch, CHAR_DATA *victim));
void jutsu_tornado args ((CHAR_DATA *ch));
void jutsu_eagra args ((CHAR_DATA *ch));
void jutsu_inos args ((CHAR_DATA *ch, CHAR_DATA *victim));
void jutsu_yaoe args ((CHAR_DATA *ch, CHAR_DATA *victim));
void jutsu_easpi args ((CHAR_DATA *ch, CHAR_DATA *victim));
void jutsu_eshina args ((CHAR_DATA *ch));
void jutsu_sha args ((CHAR_DATA *ch));
void jutsu_tri args ((CHAR_DATA *ch));
void jutsu_shoju args ((CHAR_DATA *ch, char *argment));
void jutsu_mian args ((CHAR_DATA *ch, CHAR_DATA *victim));
void jutsu_goazento args ((CHAR_DATA *ch));
void jutsu_hishu args ((CHAR_DATA *ch, CHAR_DATA *victim));
void jutsu_beoutu args ((CHAR_DATA *ch));
void jutsu_yu args ((CHAR_DATA *ch));
void jutsu_soothe args ((CHAR_DATA *ch, CHAR_DATA *victim));
void jutsu_wash args ((CHAR_DATA *ch, CHAR_DATA *victim));
void jutsu_fluidity args ((CHAR_DATA *ch));
void jutsu_aqua args ((CHAR_DATA *ch, CHAR_DATA *tarchar));
void jutsu_viunemto args ((CHAR_DATA *ch, CHAR_DATA *victim));
void jutsu_storm args ((CHAR_DATA *ch));
void jutsu_apprentice args ((CHAR_DATA *ch, CHAR_DATA *victim));

void do_jutsus(CHAR_DATA *ch, char *argument)
{
	char move[MSL];
	char target[MSL];
	CHAR_DATA *tarchar;
	CHAR_DATA *gtarchar;

	int notarg = FALSE;
	int selftarg = FALSE;
	int dir;

	argument = one_argument(argument,move);
	argument = one_argument(argument,target);

	if(IS_NPC(ch)) return;
	
	if(!IS_CLASS(ch,CLASS_SHINOBI)){
		stc("Huh?\n\r",ch);
		return;
	}

	if(move[0] == '\0' && ch->fighting == NULL){
		// Just viewing commands.
		stc("#0\\|/\\|/\\|/\\|/\\|/\\|/\\|/\\|/\\|/\\|/\\|/\\|/\\|/\\|/\\|/\\|/\\|/\\|/\\|/\\|/\\|/\\|/\\|/\\|/\n\r",ch);
		stc(" #0|				#lJUTSUS#0			              |\n\r",ch);
		stc("#0\\|/\\|/\\|/\\|/\\|/\\|/\\|/\\|/\\|/\\|/\\|/\\|/\\|/\\|/\\|/\\|/\\|/\\|/\\|/\\|/\\|/\\|/\\|/\\|/\n\r",ch);
		stc(" #0|       #RFire Jutsus-                                                 #0|\n\r",ch);
		if(ch->pcdata->powers[S_HANDFIRE] >= 1)
		stc(" #0|       #RFagra #r- Shoot a bolt of flame at your foe.                   #0|\n\r",ch);
		if(ch->pcdata->powers[S_HANDFIRE] >= 2)
		stc(" #0|       #RTorra #r- Create a wall of shurikens.                          #0|\n\r",ch);
		if(ch->pcdata->powers[S_HANDFIRE] >= 3)
		stc(" #0|       #RBae #r- You can blind your foe.                                #0|\n\r",ch);
		if(ch->pcdata->powers[S_HANDFIRE] >= 4)
		stc(" #0|       #RTaiegra #r- Create a defensive shield of fire.                 #0|\n\r",ch);
		if(ch->pcdata->powers[S_HANDFIRE] >= 5)
		stc(" #0|       #REnnotate #r- A powerful blade of fire.                         #0|\n\r",ch);
		if(ch->pcdata->powers[S_HANDFIRE] >= 6)
		stc(" #0|       #RLava #r- Fill a room with deadly lava.                         #0|\n\r",ch);

		stc(" #0||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n\r",ch);		
		stc(" #0|       #0Shadow Jutsus-                                               #0|\n\r",ch);
		if(ch->pcdata->powers[S_HANDSHADOW] >= 1)
		stc(" #0|       #0Vensai #0- Travel to your victim from anywhere.                #0|\n\r",ch);
		if(ch->pcdata->powers[S_HANDSHADOW] >= 2)
		stc(" #0|       #0Butomi #0- Create images of yourself.                          #0|\n\r",ch);
		if(ch->pcdata->powers[S_HANDSHADOW] >= 3)
		stc(" #0|       #0Sami #0- Become invisible to the world.                        #0|\n\r",ch);
		if(ch->pcdata->powers[S_HANDSHADOW] >= 4)
		stc(" #0|       #0SauTchen #0- Attack with damaging shadow chains.               #0|\n\r",ch);
		if(ch->pcdata->powers[S_HANDSHADOW] >= 5)
		stc(" #0|       #0Daemi #0- Infuse yourself with demon essence.                  #0|\n\r",ch);
		if(ch->pcdata->powers[S_HANDSHADOW] >= 6)
		stc(" #0|       #0SauButomi #0- Create powerful clones of yourself               #0|\n\r",ch);

		stc(" #0||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n\r",ch);		
		stc(" #0|       #CWind Jutsus-                                                 #0|\n\r",ch);
		if(ch->pcdata->powers[S_HANDWIND] >= 1)
		stc(" #0|       #CUpdraft #c- Travel to your foe.                                #0|\n\r",ch);
		if(ch->pcdata->powers[S_HANDWIND] >= 2)
		stc(" #0|       #CAero #c- Attack everyone around you with wind.                 #0|\n\r",ch);
		if(ch->pcdata->powers[S_HANDWIND] >= 3)
		stc(" #0|       #CHoana #c- Locate your victim from the world.                   #0|\n\r",ch);
		if(ch->pcdata->powers[S_HANDWIND] >= 4)
		stc(" #0|       #CChagi #c- Make your victim hallucinate.                        #0|\n\r",ch);
		if(ch->pcdata->powers[S_HANDWIND] >= 5)
		stc(" #0|       #CAeblade #c- A powerful blade of wind.                          #0|\n\r",ch);
		if(ch->pcdata->powers[S_HANDWIND] >= 6)
		stc(" #0|       #CTornado #c- Fill the room with a mighty tornado.               #0|\n\r",ch);

		stc(" #0||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n\r",ch);		
		stc(" #0|       #LWater Jutsus-                                                #0|\n\r",ch);
		if(ch->pcdata->powers[S_HANDWATER] >= 1)
		stc(" #0|       #LSoothe #l- Heal hp.                                            #0|\n\r",ch);
		if(ch->pcdata->powers[S_HANDWATER] >= 2)
		stc(" #0|       #LWash #l- Your enemy loses a few hits per round.                #0|\n\r",ch);
		if(ch->pcdata->powers[S_HANDWATER] >= 3)
		stc(" #0|       #LFluidity #l- Increase your parry dodge, and gain attacks       #0|\n\r",ch);
		if(ch->pcdata->powers[S_HANDWATER] >= 4)
		stc(" #0|       #LAqua #l- A powerful blade of water.                            #0|\n\r",ch);
		if(ch->pcdata->powers[S_HANDWATER] >= 5)
		stc(" #0|       #LViuNemTo #l- Ambush your foe.                                  #0|\n\r",ch);
		if(ch->pcdata->powers[S_HANDWATER] >= 6)
		stc(" #0|       #LStorm #l- Summon a storm of lethal strength.                   #0|\n\r",ch);

		stc(" #0||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n\r",ch);		
		stc(" #0|       #7Holy Jutsus-                                                 #0|\n\r",ch);
		if(ch->pcdata->powers[S_HANDHOLY] >= 1)
		stc(" #0|       #7Shoju #n- Infuse a weapon with shinobi strength.               #0|\n\r",ch);
		if(ch->pcdata->powers[S_HANDHOLY] >= 2)
		stc(" #0|       #7Mian #n- Heal a player and their soul.                         #0|\n\r",ch);
		if(ch->pcdata->powers[S_HANDHOLY] >= 3)
		stc(" #0|       #7GoaZenTo #n- Turn the room you are in, into a sanctuary.       #0|\n\r",ch);
		if(ch->pcdata->powers[S_HANDHOLY] >= 4)
		stc(" #0|       #7Hishu #n- A powerful blast of holy energy.                     #0|\n\r",ch);
		if(ch->pcdata->powers[S_HANDHOLY] >= 5)
		stc(" #0|       #7Beoutu #n- Bless your shinobi friends.                         #0|\n\r",ch);
		if(ch->pcdata->powers[S_HANDHOLY] >= 6)
		stc(" #0|       #7Yu #n- Fill the room with holy energies.                       #0|\n\r",ch);

		stc(" #0||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n\r",ch);		
		stc(" #0|       #yEarth Jutsus-                                                #0|\n\r",ch);
		if(ch->pcdata->powers[S_HANDEARTH] >= 1)
		stc(" #0|       #yEagra #o- Shield yourself from mortal eyes.                    #0|\n\r",ch);
		if(ch->pcdata->powers[S_HANDEARTH] >= 2)
		stc(" #0|       #yInos #o- Have the earth swallow your victim.                   #0|\n\r",ch);
		if(ch->pcdata->powers[S_HANDEARTH] >= 3)
		stc(" #0|       #yYaoe #o- Cover your victim in a thick clay.                    #0|\n\r",ch);
		if(ch->pcdata->powers[S_HANDEARTH] >= 4)
		stc(" #0|       #yEaspi #o- A powerful earthen blade.                            #0|\n\r",ch);
		if(ch->pcdata->powers[S_HANDEARTH] >= 5)
		stc(" #0|       #yEshina #o- Become immune to sharp weapons.                     #0|\n\r",ch);
		if(ch->pcdata->powers[S_HANDEARTH] >= 6)
		stc(" #0|       #ySha #o- Turn the room to a barren waste.                       #0|\n\r",ch);

			stc(" #0||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n\r",ch);

		if(ch->pcdata->powers[S_RANK] >= S_JOUNIN){
			stc(" #0|       #PApprentice - Make another shinobi.                           #0|\n\r",ch);
		}
		
		stc(" #0|       #PTri - When 3 or more shinobi are in the room...              #0|\n\r",ch);
		stc("#0/|\\/|\\/|\\/|\\/|\\/|\\/|\\/|\\/|\\/|\\/|\\/|\\/|\\/|\\/|\\/|\\/|\\/|\\/|\\/|\\/|\\/|\\/|\\/|\\\n\r",ch);
		return;
	}

	notarg = FALSE;
	if(target[0] == '\0'){
		if( (tarchar = ch->fighting) == NULL){
			notarg = TRUE;
		}
	}
	else{
		if( (tarchar = get_char_room(ch, NULL, target)) == NULL){
			notarg = TRUE;
		}
		if(!str_cmp(move,"vensai") || !str_cmp(move,"updraft") || !str_cmp(move,"hoana") || !str_cmp(move,"viunemto")){
			if((gtarchar = get_char_world(ch,target)) != NULL){
				notarg = 3;
			}
			else notarg = FALSE;
		}
	}

	if(notarg == FALSE && ch == tarchar) selftarg = TRUE;

	
	//dir = is_number(target) ? atoi(target) : -1;
	if(!str_cmp(target,"n")) dir = 0;
	if(!str_cmp(target,"e")) dir = 1;
	if(!str_cmp(target,"s")) dir = 2;
	if(!str_cmp(target,"w")) dir = 3;
	if(!str_cmp(target,"u")) dir = 4;
	if(!str_cmp(target,"d")) dir = 5;

	if(!str_cmp(move,"butomi") && IS_SET(ch->in_room->room_flags, ROOM_ASTRAL)){
		stc("Not here.\n\r",ch);
		return;
	}

	if(!str_cmp(move,"fagra") && ch->pcdata->powers[S_HANDFIRE] >= 1 && notarg == FALSE && selftarg == FALSE) jutsu_fagra(ch,tarchar);
	else if(!str_cmp(move,"fagra") && ch->pcdata->powers[S_HANDFIRE] >= 1 && (notarg == TRUE|| selftarg == TRUE)) JFAIL
	else if(!str_cmp(move,"torra") && ch->pcdata->powers[S_HANDFIRE] >= 2 && dir > -1 && dir < 6) jutsu_torra(ch,dir);
	else if(!str_cmp(move,"torra") && ch->pcdata->powers[S_HANDFIRE] >= 2 && (dir <= -1 || dir >= 6)) JFAIL
	else if(!str_cmp(move,"bae") && ch->pcdata->powers[S_HANDFIRE] >= 3 && notarg == FALSE && selftarg == FALSE) jutsu_bae(ch,tarchar);
	else if(!str_cmp(move,"bae") && ch->pcdata->powers[S_HANDFIRE] >= 3 && (notarg == TRUE || selftarg == TRUE)) JFAIL
 	else if(!str_cmp(move,"taiegra") && ch->pcdata->powers[S_HANDFIRE] >= 4) jutsu_taiegra(ch);
	else if(!str_cmp(move,"ennotate") && ch->pcdata->powers[S_HANDFIRE] >= 5 && notarg == FALSE && selftarg == FALSE) jutsu_ennotate(ch,tarchar);
	else if(!str_cmp(move,"ennotate") && ch->pcdata->powers[S_HANDFIRE] >= 5 && (notarg == TRUE || selftarg == TRUE)) JFAIL
 	else if(!str_cmp(move,"lava") && ch->pcdata->powers[S_HANDFIRE] >= 6) jutsu_lava(ch);
	else if(!str_cmp(move,"vensai") && ch->pcdata->powers[S_HANDSHADOW] >= 1 && notarg == 3 && selftarg == FALSE) jutsu_vensai(ch,gtarchar);
	else if(!str_cmp(move,"vensai") && ch->pcdata->powers[S_HANDSHADOW] >= 1 && (notarg != 3 || selftarg == TRUE)) JFAIL
 	else if(!str_cmp(move,"butomi") && ch->pcdata->powers[S_HANDSHADOW] >= 2) jutsu_butomi(ch);
 	else if(!str_cmp(move,"sami") && ch->pcdata->powers[S_HANDSHADOW] >= 3) jutsu_sami(ch);
	else if(!str_cmp(move,"sautchen") && ch->pcdata->powers[S_HANDSHADOW] >= 4 && notarg == FALSE && selftarg == FALSE) jutsu_sautchen(ch,tarchar);
	else if(!str_cmp(move,"sautchen") && ch->pcdata->powers[S_HANDSHADOW] >= 4 && (notarg == TRUE || selftarg == TRUE)) JFAIL
 	else if(!str_cmp(move,"daemi") && ch->pcdata->powers[S_HANDSHADOW] >= 5) jutsu_daemi(ch);
	else if(!str_cmp(move,"updraft") && ch->pcdata->powers[S_HANDWIND] >= 1 && notarg == 3 && selftarg == FALSE) jutsu_updraft(ch,gtarchar);
	else if(!str_cmp(move,"updraft") && ch->pcdata->powers[S_HANDWIND] >= 1 && (notarg != 3 || selftarg == TRUE)) JFAIL
 	else if(!str_cmp(move,"aero") && ch->pcdata->powers[S_HANDWIND] >= 2) jutsu_aero(ch);
	else if(!str_cmp(move,"hoana") && ch->pcdata->powers[S_HANDWIND] >= 3 && notarg == 3 && selftarg == FALSE) jutsu_hoana(ch,gtarchar);
	else if(!str_cmp(move,"hoana") && ch->pcdata->powers[S_HANDWIND] >= 3 && (notarg != 3 || selftarg == TRUE)) JFAIL
	else if(!str_cmp(move,"chagi") && ch->pcdata->powers[S_HANDWIND] >= 4 && notarg == FALSE && selftarg == FALSE) jutsu_chagi(ch,tarchar);
	else if(!str_cmp(move,"chagi") && ch->pcdata->powers[S_HANDWIND] >= 4 && (notarg == TRUE || selftarg == TRUE)) JFAIL
	else if(!str_cmp(move,"aeblade") && ch->pcdata->powers[S_HANDWIND] >= 5 && notarg == FALSE && selftarg == FALSE) jutsu_aeblade(ch,tarchar);
	else if(!str_cmp(move,"aeblade") && ch->pcdata->powers[S_HANDWIND] >= 5 && (notarg == TRUE || selftarg == TRUE)) JFAIL
 	else if(!str_cmp(move,"tornado") && ch->pcdata->powers[S_HANDWIND] >= 6) jutsu_tornado(ch);
 	else if(!str_cmp(move,"eagra") && ch->pcdata->powers[S_HANDEARTH] >= 1) jutsu_eagra(ch);
	else if(!str_cmp(move,"inos") && ch->pcdata->powers[S_HANDEARTH] >= 2 && notarg == FALSE && selftarg == FALSE) jutsu_inos(ch,tarchar);
	else if(!str_cmp(move,"inos") && ch->pcdata->powers[S_HANDEARTH] >= 2 && (notarg == TRUE || selftarg == TRUE)) JFAIL
	else if(!str_cmp(move,"yaoe") && ch->pcdata->powers[S_HANDEARTH] >= 3 && notarg == FALSE && selftarg == FALSE) jutsu_yaoe(ch,tarchar);
	else if(!str_cmp(move,"yaoe") && ch->pcdata->powers[S_HANDEARTH] >= 3 && (notarg == TRUE || selftarg == TRUE)) JFAIL
	else if(!str_cmp(move,"easpi") && ch->pcdata->powers[S_HANDEARTH] >= 4 && notarg == FALSE && selftarg == FALSE) jutsu_easpi(ch,tarchar);
	else if(!str_cmp(move,"easpi") && ch->pcdata->powers[S_HANDEARTH] >= 4 && (notarg == TRUE || selftarg == TRUE)) JFAIL
 	else if(!str_cmp(move,"eshina") && ch->pcdata->powers[S_HANDEARTH] >= 5) jutsu_eshina(ch);
 	else if(!str_cmp(move,"sha") && ch->pcdata->powers[S_HANDEARTH] >= 6) jutsu_sha(ch);
	else if(!str_cmp(move,"shoju") && ch->pcdata->powers[S_HANDHOLY] >= 1) jutsu_shoju(ch,target);
	else if(!str_cmp(move,"mian") && ch->pcdata->powers[S_HANDHOLY] >= 2 && notarg == FALSE) jutsu_mian(ch,tarchar);
	else if(!str_cmp(move,"mian") && ch->pcdata->powers[S_HANDHOLY] >= 2 && notarg == TRUE) JFAIL
 	else if(!str_cmp(move,"goazento") && ch->pcdata->powers[S_HANDHOLY] >= 3) jutsu_goazento(ch);
	else if(!str_cmp(move,"hishu") && ch->pcdata->powers[S_HANDHOLY] >= 4 && notarg == FALSE && selftarg == FALSE) jutsu_hishu(ch,tarchar);
	else if(!str_cmp(move,"hishu") && ch->pcdata->powers[S_HANDHOLY] >= 4 && (notarg == TRUE || selftarg == TRUE)) JFAIL
 	else if(!str_cmp(move,"beoutu") && ch->pcdata->powers[S_HANDHOLY] >= 5) jutsu_beoutu(ch);
 	else if(!str_cmp(move,"yu") && ch->pcdata->powers[S_HANDHOLY] >= 6) jutsu_yu(ch);
	else if(!str_cmp(move,"soothe") && ch->pcdata->powers[S_HANDWATER] >= 1 && notarg == FALSE) jutsu_soothe(ch,tarchar);
	else if(!str_cmp(move,"soothe") && ch->pcdata->powers[S_HANDWATER] >= 1 && (notarg == TRUE)) JFAIL
	else if(!str_cmp(move,"wash") && ch->pcdata->powers[S_HANDWATER] >= 2 && notarg == FALSE && selftarg == FALSE) jutsu_wash(ch,tarchar);
	else if(!str_cmp(move,"wash") && ch->pcdata->powers[S_HANDWATER] >= 2 && (notarg == TRUE || selftarg == TRUE)) JFAIL
 	else if(!str_cmp(move,"fluidity") && ch->pcdata->powers[S_HANDWATER] >= 3) jutsu_fluidity(ch);
	else if(!str_cmp(move,"aqua") && ch->pcdata->powers[S_HANDWATER] >= 4 && notarg == FALSE && selftarg == FALSE) jutsu_aqua(ch,tarchar);
	else if(!str_cmp(move,"aqua") && ch->pcdata->powers[S_HANDWATER] >= 4 && (notarg == TRUE || selftarg == TRUE)) JFAIL
	else if(!str_cmp(move,"viunemto") && ch->pcdata->powers[S_HANDWATER] >= 5 && notarg == 3 && selftarg == FALSE) jutsu_viunemto(ch,gtarchar);
	else if(!str_cmp(move,"viunemto") && ch->pcdata->powers[S_HANDWATER] >= 5 && (notarg != 3 || selftarg == TRUE)) JFAIL
 	else if(!str_cmp(move,"storm") && ch->pcdata->powers[S_HANDWATER] >= 6) jutsu_storm(ch);
	else if(!str_cmp(move,"apprentice") && ch->pcdata->powers[S_RANK] >= S_JOUNIN && notarg == FALSE && selftarg == FALSE) jutsu_apprentice(ch,tarchar);
	else if(!str_cmp(move,"apprentice") && ch->pcdata->powers[S_RANK] >= S_JOUNIN && (notarg == TRUE || selftarg == TRUE)) JFAIL
 	else if(!str_cmp(move,"tri")) jutsu_tri(ch);
	else stc("That is not a valid jutsu move.\n\r",ch);
	return;

}

void do_handmotions(CHAR_DATA *ch, char *argument)
{

	char arg[MSL];
	char buf[MSL];
	int found = FALSE;
	int price = 0;
	argument = one_argument(argument,arg);

	if(IS_NPC(ch)) return;

	if(!IS_CLASS(ch,CLASS_SHINOBI)){
		stc("Huh?\n\r",ch);
		return;
	}
	if(!str_cmp(arg,"blah") && ch->level > 7){
		ch->pcdata->powers[S_HANDFIRE] = 0;
		ch->pcdata->powers[S_HANDWATER] = 0;
		ch->pcdata->powers[S_HANDEARTH] = 0;
		ch->pcdata->powers[S_HANDWIND] = 0;
		ch->pcdata->powers[S_HANDHOLY] = 0;
		ch->pcdata->powers[S_HANDSHADOW] = 0;
		return;
	}
	if(arg[0] == '\0'){
		stc("Fire, Earth, Water, Wind, Holy, Shadow\n\r",ch);
		return;
	}
	if(str_cmp(arg,"water") && str_cmp(arg,"earth") && 
str_cmp(arg,"fire") && str_cmp(arg,"wind") && str_cmp(arg,"holy") && 
str_cmp(arg,"shadow")){
		stc("Fire, EArth, Water, Wind, Holy, Shadow\n\r",ch);
		return;
	}
	switch(ch->pcdata->powers[S_ELEMENT]){
		case S_FIRE:
			if(!str_cmp(arg,"water")) found = FALSE;
			else if(!str_cmp(arg,"fire") && ch->pcdata->powers[S_HANDFIRE] >= 6) found = FALSE;
			else if(!str_cmp(arg,"shadow") && ch->pcdata->powers[S_HANDSHADOW] >= 3) found = FALSE;
			else if(!str_cmp(arg,"earth") && ch->pcdata->powers[S_HANDEARTH] >= 3) found = FALSE;
			else if(!str_cmp(arg,"wind") && ch->pcdata->powers[S_HANDWIND] >= 1) found = FALSE;
			else if(!str_cmp(arg,"holy") && ch->pcdata->powers[S_HANDHOLY] >= 1) found = FALSE;
			else found = TRUE;
			break;
		case S_SHADOW:
			if(!str_cmp(arg,"holy")) found = FALSE;
			else if(!str_cmp(arg,"shadow") && ch->pcdata->powers[S_HANDSHADOW] >= 6) found = FALSE;
			else if(!str_cmp(arg,"fire") && ch->pcdata->powers[S_HANDFIRE] >= 3) found = FALSE;
			else if(!str_cmp(arg,"wind") && ch->pcdata->powers[S_HANDWIND] >= 3) found = FALSE;
			else if(!str_cmp(arg,"water") && ch->pcdata->powers[S_HANDWATER] >= 1) found = FALSE;
			else if(!str_cmp(arg,"earth") && ch->pcdata->powers[S_HANDEARTH] >= 1) found = FALSE;
			else found = TRUE;
			break;
		case S_WIND:
			if(!str_cmp(arg,"earth")) found = FALSE;
			else if(!str_cmp(arg,"wind") && ch->pcdata->powers[S_HANDWIND] >= 6) found = FALSE;
			else if(!str_cmp(arg,"water") && ch->pcdata->powers[S_HANDWATER] >= 3) found = FALSE;
			else if(!str_cmp(arg,"shadow") && ch->pcdata->powers[S_HANDSHADOW] >= 3) found = FALSE;
			else if(!str_cmp(arg,"holy") && ch->pcdata->powers[S_HANDHOLY] >= 1) found = FALSE;
			else if(!str_cmp(arg,"fire") && ch->pcdata->powers[S_HANDFIRE] >= 1) found = FALSE;
			else found = TRUE;
			break;
		case S_WATER:
			if(!str_cmp(arg,"fire")) found = FALSE;
			else if(!str_cmp(arg,"water") && ch->pcdata->powers[S_HANDWATER] >= 6) found = FALSE;
			else if(!str_cmp(arg,"wind") && ch->pcdata->powers[S_HANDWIND] >= 3) found = FALSE;
			else if(!str_cmp(arg,"holy") && ch->pcdata->powers[S_HANDHOLY] >= 3) found = FALSE;
			else if(!str_cmp(arg,"earth") && ch->pcdata->powers[S_HANDEARTH] >= 1) found = FALSE;
			else if(!str_cmp(arg,"shadow") && ch->pcdata->powers[S_HANDSHADOW] >= 1) found = FALSE;
			else found = TRUE;
			break;
		case S_HOLY:
			if(!str_cmp(arg,"shadow")) found = FALSE;
			else if(!str_cmp(arg,"holy") && ch->pcdata->powers[S_HANDHOLY] >= 6) found = FALSE;
			else if(!str_cmp(arg,"earth") && ch->pcdata->powers[S_HANDEARTH] >= 3) found = FALSE;
			else if(!str_cmp(arg,"water") && ch->pcdata->powers[S_HANDWATER] >= 3) found = FALSE;
			else if(!str_cmp(arg,"fire") && ch->pcdata->powers[S_HANDFIRE] >= 1) found = FALSE;
			else if(!str_cmp(arg,"wind") && ch->pcdata->powers[S_HANDWIND] >= 1) found = FALSE;
			else found = TRUE;
			break;
		case S_EARTH:
			if(!str_cmp(arg,"wind")) found = FALSE;
			else if(!str_cmp(arg,"earth") && ch->pcdata->powers[S_HANDEARTH] >= 6) found = FALSE;
			else if(!str_cmp(arg,"holy") && ch->pcdata->powers[S_HANDHOLY] >= 3) found = FALSE;
			else if(!str_cmp(arg,"fire") && ch->pcdata->powers[S_HANDFIRE] >= 3) found = FALSE;
			else if(!str_cmp(arg,"water") && ch->pcdata->powers[S_HANDWATER] >= 1) found = FALSE;
			else if(!str_cmp(arg,"shadow") && ch->pcdata->powers[S_HANDSHADOW] >= 1) found = FALSE;
			else found = TRUE;
			break;
	}

	if(found == FALSE){
		stc("You can not learn any more in that handmotion.\n\r",ch);
		return;
	}

	if(!str_cmp(arg,"earth")) price = ch->pcdata->powers[S_HANDEARTH] * 500 + 500;
	if(!str_cmp(arg,"fire")) price = ch->pcdata->powers[S_HANDFIRE] * 500 + 500;
	if(!str_cmp(arg,"wind")) price = ch->pcdata->powers[S_HANDWIND] * 500 + 500;
	if(!str_cmp(arg,"water")) price = ch->pcdata->powers[S_HANDWATER] * 500 + 500;
	if(!str_cmp(arg,"holy")) price = ch->pcdata->powers[S_HANDHOLY] * 500 + 500;
	if(!str_cmp(arg,"shadow")) price = ch->pcdata->powers[S_HANDSHADOW] * 500 + 500;

	if(price > ch->practice){
		sprintf(buf, "You need %d primal to learn your next handmotion.\n\r", price);
		stc(buf,ch);
		return;
	}

	if(price <= 499) price = 500;
	ch->practice -= price;

	if(!str_cmp(arg,"shadow")) ch->pcdata->powers[S_HANDSHADOW] += 1;
	if(!str_cmp(arg,"wind")) ch->pcdata->powers[S_HANDWIND] += 1;
	if(!str_cmp(arg,"fire")) ch->pcdata->powers[S_HANDFIRE] += 1;
	if(!str_cmp(arg,"water")) ch->pcdata->powers[S_HANDWATER] += 1;
	if(!str_cmp(arg,"holy")) ch->pcdata->powers[S_HANDHOLY] += 1;
	if(!str_cmp(arg,"earth")) ch->pcdata->powers[S_HANDEARTH] += 1;
	sprintf(buf, "You learn more %s handmotions.\n\r", arg);
	stc(buf,ch);
	return;

}


void do_shinobirank(CHAR_DATA *ch, char *argument)
{
	
	char buf[MSL];

	if(IS_NPC(ch)) return;
	if(!IS_CLASS(ch,CLASS_SHINOBI)){
		stc("Huh?\n\r",ch);
		return;
	}
	if(ch->pcdata->powers[S_RANK] <= S_APPRENTICE &&
	ch->max_hit >= 100000 && ch->max_move >= 50000 && ch->max_mana >= 50000 &&
	(ch->chakra / 1000) >= 3){
		sprintf(buf, "%s has become a Genin!", ch->pcdata->switchname);
		do_info(ch,buf);
		ch->pcdata->powers[S_RANK] = S_GENIN;
		stc("You advance in shinobi rank.\n\r",ch);
		return;
	}
	if(ch->pcdata->powers[S_RANK] <= S_GENIN &&
	ch->max_hit >= 120000 && ch->max_move >= 50000 && ch->max_mana >= 50000 &&
	(ch->chakra / 1000) >= 6){
		sprintf(buf, "%s has become a Jounin!", ch->pcdata->switchname);
		do_info(ch,buf);
		ch->pcdata->powers[S_RANK] = S_JOUNIN;
		stc("You advance in shinobi rank.\n\r",ch);
		return;
	}
	if(ch->pcdata->powers[S_RANK] <= S_JOUNIN &&
	ch->max_hit >= 180000 && ch->max_move >= 50000 && ch->max_mana >= 50000 &&
	(ch->chakra / 1000) >= 10){
		sprintf(buf, "%s has become a Kage!", ch->pcdata->switchname);
		do_info(ch,buf);
		ch->pcdata->powers[S_RANK] = S_KAGE;
		stc("You advance in shinobi rank.\n\r",ch);
		return;
	}
	stc("You can not obtain a new rank at this time.\n\r",ch);
	return;

}

//Fire Powers
void jutsu_fagra(CHAR_DATA *ch, CHAR_DATA *tarchar)
{
	char buf[MSL];

	int dam;
	dam = ch->pRank * 300;
	dam *= ch->pcdata->powers[S_RANK] + 1;
	dam /= 1;

	if(is_safe(ch,tarchar) == TRUE){
		stc("Not in this room.\n\r",ch);
		return;
	}
	sprintf(buf,"You shoot a blast of flame at %s! [#R%d#n]\n\r",GET_PROPER_NAME(tarchar),dam);
	stc(buf,ch);
	sprintf(buf,"You are shot by a blast of flame from %s! [#R%d#n]\n\r",GET_PROPER_NAME(ch),dam);
	stc(buf,tarchar);
	set_fighting(ch,tarchar, TRUE);
	hurt_person(ch,tarchar,dam);
	WAIT_STATE(ch,12);
	
}

void jutsu_torra(CHAR_DATA *ch, int dir)
{

	if(ch->in_room->exit[dir] == NULL){
		stc("You can not create a wall there.\n\r",ch);
		return;
	}
	act("$n tosses shurikens into the air and suspends them with columns of hot air.",ch,NULL,NULL,TO_ROOM);
	stc("You create a wall of shurikens!\n\r",ch);	
	make_wall(ch->in_room,dir,EX_SHIN_WALL);
	WAIT_STATE(ch,12);

}

void jutsu_bae(CHAR_DATA *ch, CHAR_DATA *tarchar)
{

	AFFECT_DATA af;

	if(is_safe(ch,tarchar) == TRUE){
		stc("You cant do that.\n\r",ch);
		return;
	}
      act("$n shoots flames into $N's eyes!'.",ch,NULL,tarchar,TO_NOTVICT);
      act("You shoot flames into $N's eyes!",ch,NULL,tarchar,TO_CHAR);
      act("$n shoots flames into your eyes, blinding you!#n'.",ch,NULL,tarchar,TO_VICT);
      if (IS_SET(tarchar->act,PLR_HOLYLIGHT)) REMOVE_BIT(tarchar->act,PLR_HOLYLIGHT);
      if (IS_SET(tarchar->affected_by, AFF_DETECT_HIDDEN))
        REMOVE_BIT(tarchar->affected_by, AFF_DETECT_HIDDEN);
      if (IS_SET(tarchar->affected_by, AFF_DETECT_INVIS))
        REMOVE_BIT(tarchar->affected_by, AFF_DETECT_INVIS);
      af.type      = skill_lookup("blindness");
      af.location  = APPLY_HITROLL;
      af.modifier  = -1;
      af.duration  = 10;
      af.bitvector = AFF_BLIND;
      affect_to_char( tarchar, &af );
	set_fighting(ch,tarchar, TRUE);
	set_fighting(tarchar,ch, TRUE);
	WAIT_STATE(ch,6);
	
}

void jutsu_taiegra(CHAR_DATA *ch)
{

	if(IS_SET(ch->pcdata->powers[S_ACTIVE], S_TAIEGRA)){
		REMOVE_BIT(ch->pcdata->powers[S_ACTIVE], S_TAIEGRA);
		stc("You banish your shield of flames.\n\r",ch);
	        act("$n deftly makes a few handmotions, and banishes their shield of flames.",ch,NULL,NULL,TO_ROOM);
	}
	else{
		SET_BIT(ch->pcdata->powers[S_ACTIVE], S_TAIEGRA);
		stc("You summon a shield of flames.\n\r",ch);
	        act("$n deftly makes a few handmotions, and conjures up a shield of flames.",ch,NULL,NULL,TO_ROOM);
	}

}

void jutsu_ennotate(CHAR_DATA *ch, CHAR_DATA *tarchar)
{

	char buf[MSL];
	int x;
	int dam;
	int dam2;
	int r;
	dam = ch->pRank * 900;
	dam *= ch->pcdata->powers[S_RANK] + 1;
	dam /= 3;

	if(is_safe(ch,tarchar) == TRUE){
		stc("Not in this room.\n\r",ch);
		return;
	}
	r = number_range(3,6);
	for(x = 1;x < r;x++){
		dam2 = number_range(dam * .1, dam) / x ;
		sprintf(buf,"%s is engulfed in a pillar of flames! [#R%d#n]\n\r",GET_PROPER_NAME(tarchar),dam2);
		stc(buf,ch);
		stc(buf,tarchar);
		hurt_person(ch,tarchar,dam2);
		if(tarchar->hit < 1) break;
	}
	set_fighting(ch,tarchar, TRUE);
	WAIT_STATE(ch,12);
	
}

void jutsu_lava(CHAR_DATA *ch)
{
/*
        if (3000 <= ch->in_room->vnum && ch->in_room->vnum <= 4000)
        {
         stc("Not in Midgaard!\n\r",ch);
         return;
        }
*/
	if(ch->pcdata->powers[S_TIMER] > 0){
		stc("You can not summon more lava yet.\n\r",ch);
		return;
	}
	if(IS_SET(ch->in_room->shinobi, ROOM_SFIRE)){
		REMOVE_BIT(ch->in_room->shinobi, ROOM_SFIRE);
		stc("You empty the room of lava.\n\r",ch);
		SET_RTIMER(ch->in_room, RTIMER_FIRE, 0);
	        act("$n deftly makes a few handmotions, and the lava vanishes.",ch,NULL,NULL,TO_ROOM);
	}
	else{
		SET_BIT(ch->in_room->shinobi, ROOM_SFIRE);
		SET_RTIMER(ch->in_room, RTIMER_FIRE, 10);
	        ch->in_room->next_update = update_room_list;
	        update_room_list = ch->in_room;
		stc("You summon lava up from the earth.\n\r",ch);
	        act("$n deftly makes a few handmotions, and lava pours forth from the ground.",ch,NULL,NULL,TO_ROOM);
		if(IS_SET(ch->in_room->shinobi, ROOM_SWATER)){
			REMOVE_BIT(ch->in_room->shinobi, ROOM_SWATER);
			act("The heat from the lava dissipates the storm.",ch,NULL,NULL,TO_ROOM);
		}
		ch->pcdata->powers[S_TIMER] = 10;
	}

}

void jutsu_vensai(CHAR_DATA *ch, CHAR_DATA *victim)
{

  if( room_is_private(victim->in_room ) ) {
    send_to_char( "That room is private right now.\n\r", ch );
    return;
  }

  if( IS_SET( victim->in_room->room_flags, ROOM_ASTRAL ) ) {
    stc( "You can't find it's room.\n\r",ch);
    return;
  }

  if( IS_SET( ch->in_room->room_flags, ROOM_ASTRAL) ) {
    stc( "Your body can't go there.\n\r",ch);
    return;
  }
	if(victim->fight_timer <= 0){
  if( !IS_NPC( victim ) && !IS_IMMUNE( victim, IMM_SUMMON ) && !ragnarok && victim->fight_timer == 0) {
    send_to_char( "They are hiding from you.\n\r", ch );
    return;
  }
  if( IS_IMMUNE(victim, IMM_TRAVEL ) && !ragnarok && victim->fight_timer == 0) {
     send_to_char("He doesnt want you near him!\n\r",ch);
     return;
  }
	}
  if( victim->in_room == ch->in_room ) {
    send_to_char( "But you're already there!\n\r",ch );
    return;
  }
  if(ch->fighting != NULL){
	stc("You cant do that in a fight!\n\r",ch);
	return;
  }
	if(victim->fight_timer > 0) WAIT_STATE(ch,7);

    char_from_room(ch);
    char_to_room(ch,victim->in_room);
    act("You merge with the shadows and appear before $N.", ch, NULL, victim, TO_CHAR);
    act("$n rises from the shadows before $N.", ch, NULL, victim, TO_NOTVICT);
    act("$n rises forth from the shadows.", ch, NULL, victim, TO_VICT);
    do_look(ch,"auto");
	return;

}

void jutsu_butomi(CHAR_DATA *ch)
{
	char buf[MSL];
	CHAR_DATA *victim;
	int r, x;	
	AFFECT_DATA af;

  if(ch->pcdata->powers[S_CLONETIMER] > 0 || ch->pcdata->followers
> 0){
                stc("You cant do that yet.\n\r",ch);
                return;	


}

	r = 1 + (ch->race / 10);



	for(x = 0;x < r; x++){
		ch->pcdata->followers++;
		if(ch->pcdata->powers[S_HANDSHADOW] < 6) victim = create_mobile( get_mob_index( 33006 ) );
		else victim = create_mobile( get_mob_index( 33007 ) );
	      victim->short_descr = str_dup(ch->name);
	      sprintf(buf, "%s is hovering here.\n\r",ch->name);
	      victim->long_descr = str_dup(buf);
	      victim->name = str_dup(ch->name);
	      victim->level = 200;
	      victim->max_hit  = ch->max_hit / 2;
		if(victim->max_hit > 600000) victim->max_hit = 600000;
	      victim->hit      = victim->max_hit;
	      victim->max_mana = ch->max_mana / 2;
	      victim->mana     = victim->max_mana;
	      victim->max_move = ch->max_move / 2;
	      victim->move     = victim->max_move;
	      victim->hitroll  = ch->hitroll;
	      victim->damroll  = ch->damroll;
	      add_follower( victim, ch );
		if(ch->pcdata->powers[S_HANDSHADOW] == 6){
  	    af.type      = skill_lookup ("charm person");
	    af.duration  = 777;
	    af.location  = APPLY_NONE;
	    af.modifier  = 0;
	    af.bitvector = AFF_CHARM;
	    affect_to_char( victim, &af );
		}
		victim->leader = ch;
		victim->master = ch;
	      SET_BIT(victim->act, ACT_NOEXP);
	      char_to_room( victim, ch->in_room );
		if(ch->fighting != NULL) set_fighting(victim,ch->fighting, TRUE);
	}

		if(ch->pcdata->powers[S_HANDSHADOW] == 6 && ch->fighting != NULL) stop_fighting(ch,TRUE);

      act( "You split your body into numerous clones of yourself.", ch, NULL, victim, TO_CHAR );
      act( "$n's body splits into numerous clones!", ch, NULL, victim, TO_ROOM );

        ch->pcdata->powers[S_CLONETIMER] = 1;

}
void jutsu_sami(CHAR_DATA *ch)
{

	if(ch->fight_timer > 0){
		stc("Not with a fighttimer.\n\r",ch);
		return;
	}
	if(!IS_SET(ch->newbits2,NEW2_HIDE)){
		stc("You turn into a shadow.\n\r",ch);
		act("$n turns into a shadow and vanishes.",ch,NULL,NULL,TO_ROOM);
		SET_BIT(ch->newbits2,NEW2_HIDE);
	}
	else{
		stc("You return to the world of light.\n\r",ch);
		act("$n appears from no where.",ch,NULL,NULL,TO_ROOM);
		REMOVE_BIT(ch->newbits2,NEW2_HIDE);
	}

}

void jutsu_sautchen(CHAR_DATA *ch, CHAR_DATA *tarchar)
{

	char buf[MSL];

	int dam, sn, level;
	dam = ch->pRank * 900;
	dam *= ch->pcdata->powers[S_RANK] + 1;
	dam /= 2;
	
	if(is_safe(ch,tarchar) == TRUE){
		stc("Not in this room.\n\r",ch);
		return;
	}
	sprintf(buf,"Chains burst forth from the ground and entangle %s! [#R%d#n]\n\r",GET_PROPER_NAME(tarchar),dam);
	stc(buf,ch);
	sprintf(buf,"Chains burst forth from the ground and entangle you! [#R%d#n]\n\r",dam);
	stc(buf,tarchar);
	set_fighting(ch,tarchar, TRUE);
	hurt_person(ch,tarchar,dam);
	if ( ( sn = skill_lookup( "web" ) ) < 0 ) return;
	level = 50;
	(*skill_table[sn].spell_fun) ( sn, level, ch, tarchar );

	WAIT_STATE(ch,12);

}
void jutsu_daemi(CHAR_DATA *ch)
{

	if(!IS_SET(ch->pcdata->powers[S_ACTIVE],S_DAEMI)){
		stc("You become one with the shadows of your soul.\n\r",ch);
		act("$n's face is covered with shadows.",ch,NULL,NULL,TO_ROOM);
		SET_BIT(ch->pcdata->powers[S_ACTIVE],S_DAEMI);
	}
	else{
		stc("You banish the shadows from your soul.\n\r",ch);
		act("The shadows on $n's face vanish.",ch,NULL,NULL,TO_ROOM);
		REMOVE_BIT(ch->pcdata->powers[S_ACTIVE],S_DAEMI);
	}

}

void jutsu_updraft(CHAR_DATA *ch, CHAR_DATA *victim)
{

  if( room_is_private(victim->in_room ) ) {
    send_to_char( "That room is private right now.\n\r", ch );
    return;
  }

  if( IS_SET( victim->in_room->room_flags, ROOM_ASTRAL ) ) {
    stc( "You can't find it's room.\n\r",ch);
    return;
  }

  if( IS_SET( ch->in_room->room_flags, ROOM_ASTRAL) ) {
    stc( "Your body can't go there.\n\r",ch);
    return;
  }
	if(victim->fight_timer <= 0){
  if( !IS_NPC( victim ) && !IS_IMMUNE( victim, IMM_SUMMON ) && !ragnarok && victim->fight_timer == 0 ) {
    send_to_char( "They are hiding from you.\n\r", ch );
    return;
  }
  if( IS_IMMUNE(victim, IMM_TRAVEL ) && !ragnarok && victim->fight_timer == 0) {
     send_to_char("He doesnt want you near him!\n\r",ch);
     return;
  }
	}
  if( victim->in_room == ch->in_room ) {
    send_to_char( "But you're already there!\n\r",ch );
    return;
  }
  if(ch->fighting != NULL){
	stc("You cant do that in a fight!\n\r",ch);
	return;
  }


	if(victim->fight_timer > 0) WAIT_STATE(ch,7);

    char_from_room(ch);
    char_to_room(ch,victim->in_room);
    act("You travel through the winds to $N.", ch, NULL, victim, TO_CHAR);
    act("$n appears from a gust of wind before $N.", ch, NULL, victim, TO_NOTVICT);
    act("$n appears from a gust of wind before you.", ch, NULL, victim, TO_VICT);
    do_look(ch,"auto");
	return;

}

void jutsu_aero(CHAR_DATA *ch)
{

	char buf[MSL];
	char buf2[MSL];
	CHAR_DATA *vch;
	CHAR_DATA *vch_next;
	int dam;
	//int dam2;
	dam = ch->pRank * 865;
	dam *= ch->pcdata->powers[S_RANK] + 1;
	dam /= 2;
	
	for ( vch = ch->in_room->people; vch!=NULL;vch = vch_next)
	      {
	        vch_next = vch->next_in_room;
	        if (can_see(ch,vch))
	        if ( IS_NPC(ch) ? !IS_NPC(vch) : IS_NPC(vch) )
	        {
		if(is_safe(ch,vch)) continue;
	          sprintf(buf, "Your body is ravaged by the tornado! [#C%d#n]\n\r", dam);
	          send_to_char( buf, vch );
	          sprintf(buf2, "%s's body is ravaged by tornadoes! [#C%d#n]\n\r",vch->name, dam);
	          send_to_char( buf2, ch );
	          set_fighting(ch,vch, TRUE);
	          hurt_person(ch,vch,dam);
	        }
	      }
	
	WAIT_STATE(ch,12);

}
void jutsu_hoana(CHAR_DATA *ch, CHAR_DATA *victim)
{
        char buf[MSL];
    ROOM_INDEX_DATA *chroom;
    ROOM_INDEX_DATA *victimroom;
    int rand_room;
    bool chaosvalue = FALSE;

/*	if(IS_ARTI(victim,ARTI_EYE) && can_see(ch,victim) && !IS_ARTI(ch,ARTI_FLUTE)){
                sprintf(buf,"%s is trying to find you!\n\r",ch->name);
                stc(buf,victim);
                stc("Your mind is seared with pain!!!\n\r",ch);
                ch->hit = ch->hit * .8;
                ch->mana = 10;
                ch->move = 10;
                WAIT_STATE(ch,48);
                ch->fight_timer += 40;
                chroom = ch->in_room;
                victimroom = victim->in_room;
                char_from_room(victim);
                char_to_room(victim,chroom);
                do_look(victim,"scry");
                char_from_room(victim);
                char_to_room(victim,victimroom);
                return;
        }
*/
    if (victim->trust > 6)
    {
      send_to_char("SPYING ON THE GODS!?!?!?!\n\r",ch);
      send_to_char("Someone is scrying you.\n\r",victim);
      return;
    }

    if (!IS_NPC(victim) && IS_IMMUNE(victim,IMM_SHIELDED) && !ragnarok)
    {
       send_to_char( "You cannot find them.\n\r",ch);
       return;
    }

    if (!IS_NPC(victim) && IS_IMMUNE(victim, IMM_SHIELD2)) chaosvalue = TRUE;

    if (IS_CLASS(ch, CLASS_VAMPIRE) && ch->pcdata->condition[COND_THIRST] < 25
        && !IS_ITEMAFF(ch, ITEMA_VISION))
    {
        send_to_char("You have insufficient blood.\n\r",ch);
        return;
    }

    if (!IS_ITEMAFF(ch, ITEMA_VISION) && IS_CLASS(ch, CLASS_VAMPIRE))
        ch->pcdata->condition[COND_THIRST] -= number_range(15,25);

    chroom = ch->in_room;
    victimroom = victim->in_room;
    if (chaosvalue && !ragnarok)
      for ( ; ; )
      {
        rand_room = number_range( 0, 65535 );
        victimroom = get_room_index( rand_room );
        if (victimroom != NULL) break;
      }

    char_from_room(ch);
    char_to_room(ch,victimroom);
    if (IS_AFFECTED(ch, AFF_SHADOWPLANE) && (!IS_AFFECTED(victim, AFF_SHADOWPLANE)))
    {
        REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
        do_look(ch,"scry");
        SET_BIT(ch->affected_by, AFF_SHADOWPLANE);
    }
    else if (!IS_AFFECTED(ch, AFF_SHADOWPLANE) && (IS_AFFECTED(victim, AFF_SHADOWPLANE)))
    {
        SET_BIT(ch->affected_by, AFF_SHADOWPLANE);
        do_look(ch,"scry");
        REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
    }
    else
        do_look(ch,"scry");
    char_from_room(ch);
    char_to_room(ch,chroom);
    if (!IS_NPC(victim)) ch->fight_timer += 5;

    if ( IS_CLASS(victim, CLASS_VAMPIRE) && IS_SET(victim->flag2, AFF_SPIRITGUARD))
    {
      act("Your spirit guardian sends you a message:",ch,NULL,victim,TO_VICT);
      act("\"$n is scrying you.\"",ch,NULL,victim,TO_VICT);
    }

}
void jutsu_chagi(CHAR_DATA *ch, CHAR_DATA *victim)
{

	if(is_safe(ch,victim)){
		stc("Not in here.\n\r",ch);
		return;
	}
	if(!IS_SET(victim->newbits2,NEW2_CHAGI)){
		stc("You cause them to hallucinate!\n\r",ch);
		stc("#sYou #sbegin #sto #shallucinate#s.#s.#s.#n\n\r",victim);
		SET_BIT(victim->newbits2,NEW2_CHAGI);
		WAIT_STATE(ch,20);
	}
	else{
		stc("You stop their hallucinations!\n\r",ch);
		stc("You stop hallucinating.\n\r",victim);
		REMOVE_BIT(victim->newbits2,NEW2_CHAGI);
		WAIT_STATE(ch,20);
	}

}
void jutsu_aeblade(CHAR_DATA *ch, CHAR_DATA *tarchar)
{
	char buf[MSL];
	int x;
	int dam;
	int dam2;
	int r;
	dam = ch->pRank * 1000;
	dam *= ch->pcdata->powers[S_RANK] + 1;
	dam /= 3;

	if(is_safe(ch,tarchar) == TRUE){
		stc("Not in this room.\n\r",ch);
		return;
	}
	r = number_range(2,4);
	for(x = 1;x < r;x++){
		dam2 = number_range(dam * .6, dam);
		sprintf(buf,"%s is ravaged by blades of wind! [#C%d#n]\n\r",GET_PROPER_NAME(tarchar),dam2);
		stc(buf,ch);
		stc(buf,tarchar);
		hurt_person(ch,tarchar,dam2);
		if(tarchar->hit < 1) break;
	}
	set_fighting(ch,tarchar, TRUE);
	WAIT_STATE(ch,12);

}
void jutsu_tornado(CHAR_DATA *ch)
{

/*        if (3000 <= ch->in_room->vnum && ch->in_room->vnum <= 4000)
        {
         stc("Not in Midgaard!\n\r",ch);
         return;
        }
*/

	if(ch->pcdata->powers[S_TIMER] > 0)
	{
		stc("You can not summon a tornado yet.\n\r",ch);
		return;
	}
	if(IS_SET(ch->in_room->shinobi, ROOM_SWIND)){
		REMOVE_BIT(ch->in_room->shinobi, ROOM_SWIND);
		stc("You banish the tornado from the room.\n\r",ch);
		SET_RTIMER(ch->in_room, RTIMER_WIND, 0);
	        act("$n deftly makes a few handmotions, and the tornado vanishes.",ch,NULL,NULL,TO_ROOM);
	}
	else{
		SET_BIT(ch->in_room->shinobi, ROOM_SWIND);
		SET_RTIMER(ch->in_room, RTIMER_WIND, 10);
	        ch->in_room->next_update = update_room_list;
	        update_room_list = ch->in_room;
		stc("You make the air spin faster and faster around you.\n\r",ch);
	        act("$n deftly makes a few handmotions, and tornados ravage the earth.",ch,NULL,NULL,TO_ROOM);
		if(IS_SET(ch->in_room->shinobi, ROOM_SEARTH)){
			REMOVE_BIT(ch->in_room->shinobi, ROOM_SEARTH);
			act("The tornado tears up the earth.",ch,NULL,NULL,TO_ROOM);
		}
		ch->pcdata->powers[S_TIMER] = 10;
	}

}

void jutsu_eagra(CHAR_DATA *ch)
{

 if (!IS_IMMUNE(ch,IMM_SHIELDED) )
  {
    send_to_char("You shield your aura from those around you.\n\r",ch);
    SET_BIT(ch->immune, IMM_SHIELDED);
    return;
  }
  send_to_char("You stop shielding your aura.\n\r",ch);
  REMOVE_BIT(ch->immune, IMM_SHIELDED);

}
void jutsu_inos(CHAR_DATA *ch,CHAR_DATA *victim)
{
  CHAR_DATA *mount;
  char van[MAX_STRING_LENGTH];
  ROOM_INDEX_DATA *pRoomIndex;
  int to_room;

  if ( victim->in_room == NULL
    ||   IS_SET(victim->in_room->room_flags, ROOM_NO_RECALL)
    ||   IS_SET(victim->in_room->room_flags, ROOM_SAFE)
    ||   IS_SET(victim->in_room->room_flags, ROOM_NO_TELEPORT))
    {
        send_to_char( "The earth can not swallow them here.\n\r", ch );
        return;
    }

  if ( number_range(0,100) > 75 ) // Vandelik has been unleashed upon Kasai's code >:)
  {
    sprintf(van,"%s failed to inos you!\n\r",ch->name); // Let em know it failed, will decrease complaints of inos.
    stc("Oops.\n\r",ch);
    stc(van,victim);
    WAIT_STATE(ch,16);  // Round and a half of lag till they can try to inos again.
    return;
  }

  for ( ; ; )
  {
    to_room = number_range( 3000, 50000 );
    pRoomIndex = get_room_index( to_room );
    if ( pRoomIndex != NULL )
    if ( !IS_SET(pRoomIndex->room_flags, ROOM_PRIVATE)
      &&   !IS_SET(pRoomIndex->room_flags, ROOM_SOLITARY)
      &&   !IS_SET(pRoomIndex->room_flags, ROOM_NO_TELEPORT)
      &&   to_room != 30008 && to_room != 30002 )
            break;
  }


  act( "$N is swallowed up by the earth.", ch, NULL, victim, TO_CHAR );
  act( "$N is swallowed up by the earth.", ch, NULL, victim, TO_ROOM );
  act( "$n concentrates and the earth swallows you up.", ch, NULL, victim, TO_VICT );
  char_from_room( victim );
  char_to_room( victim, pRoomIndex );
  do_look( victim, "auto" );
  act( "$N appears from the earth.", ch, NULL, victim, TO_CHAR );
  act( "$N appears from the earth.", ch, NULL, victim, TO_ROOM );
  act( "The earth releases you.", ch, NULL, victim, TO_VICT );
  if ( (mount = ch->mount) == NULL ) return;
  char_from_room( mount );
  char_to_room( mount, ch->in_room );
  do_look( mount, "auto" );
  ch->fight_timer += 5;
  return;

}

void jutsu_yaoe(CHAR_DATA *ch,CHAR_DATA *victim)
{
    int sn,level;

    if ( ( sn = skill_lookup( "web" ) ) < 0 ) return;
    level = 50;
    (*skill_table[sn].spell_fun) ( sn, level, ch, victim );
    WAIT_STATE( ch, 12 );
    return;

}
void jutsu_easpi(CHAR_DATA *ch,CHAR_DATA *tarchar)
{
	char buf[MSL];
	int x;
	int dam;
	int dam2;
	int r;
	OBJ_DATA *obj;
	CHAR_DATA *victim;
	dam = ch->pRank * 900;
	dam *= ch->pcdata->powers[S_RANK] + 1;
	dam /= 3;

	victim = tarchar;

	if(is_safe(ch,tarchar) == TRUE){
		stc("Not in this room.\n\r",ch);
		return;
	}
	r = 2;
	for(x = 1;x < r;x++){
		dam2 = number_range(dam * .6, dam);
		sprintf(buf,"%s is struck hard by earthen blades! [#o%d#n]\n\r",GET_PROPER_NAME(tarchar),dam2);
		stc(buf,ch);
		stc(buf,tarchar);
		hurt_person(ch,tarchar,dam2);

		if(number_range(0,100) <= 40){
		if (!IS_ARM_R(victim,LOST_ARM))            SET_BIT(victim->loc_hp[3],LOST_ARM);
		    if (!IS_BLEEDING(victim,BLEEDING_ARM_R))   SET_BIT(victim->loc_hp[6],BLEEDING_ARM_R);
		    if (IS_BLEEDING(victim,BLEEDING_HAND_R))   REMOVE_BIT(victim->loc_hp[6],BLEEDING_HAND_R);
		    if ((obj = get_eq_char( victim, WEAR_ARMS )) != NULL)     take_item(victim,obj);
		    if ((obj = get_eq_char( victim, WEAR_WIELD )) != NULL)    take_item(victim,obj);
		    if ((obj = get_eq_char( victim, WEAR_HANDS )) != NULL)    take_item(victim,obj);
		    if ((obj = get_eq_char( victim, WEAR_WRIST_R )) != NULL)  take_item(victim,obj);
		    if ((obj = get_eq_char( victim, WEAR_FINGER_R )) != NULL) take_item(victim,obj);
		    if (!IS_ARM_L(victim,LOST_ARM))            SET_BIT(victim->loc_hp[2],LOST_ARM);
		    if (!IS_BLEEDING(victim,BLEEDING_ARM_L))   SET_BIT(victim->loc_hp[6],BLEEDING_ARM_L);
		    if (IS_BLEEDING(victim,BLEEDING_HAND_L))   REMOVE_BIT(victim->loc_hp[6],BLEEDING_HAND_L);
		    if ((obj = get_eq_char( victim, WEAR_ARMS )) != NULL)     take_item(victim,obj);
		    if ((obj = get_eq_char( victim, WEAR_HOLD )) != NULL)     take_item(victim,obj);
		    if ((obj = get_eq_char( victim, WEAR_HANDS )) != NULL)    take_item(victim,obj);
		    if ((obj = get_eq_char( victim, WEAR_WRIST_L )) != NULL)  take_item(victim,obj);
		    if ((obj = get_eq_char( victim, WEAR_FINGER_L )) != NULL) take_item(victim,obj);
		}
		if(tarchar->hit < 1) break;
	}
	set_fighting(ch,tarchar, TRUE);
	WAIT_STATE(ch,12);

}
void jutsu_eshina(CHAR_DATA *ch)
{
	if(!IS_SET(ch->pcdata->powers[S_ACTIVE],S_ESHINA)){
		stc("Your limbs become immune to damage..\n\r",ch);
		act("$n's limbs turn to stone.",ch,NULL,NULL,TO_ROOM);
		SET_BIT(ch->pcdata->powers[S_ACTIVE],S_ESHINA);
	}
	else{
		stc("Your limbs soften..\n\r",ch);
		act("$n's limbs soften.",ch,NULL,NULL,TO_ROOM);
		REMOVE_BIT(ch->pcdata->powers[S_ACTIVE],S_ESHINA);
	}

}
void jutsu_sha(CHAR_DATA *ch)
{

	if(ch->pcdata->powers[S_TIMER] > 0){
		stc("You can not summon the barren wastes yet.\n\r",ch);
		return;
	}
	if(IS_SET(ch->in_room->shinobi, ROOM_SEARTH)){
		REMOVE_BIT(ch->in_room->shinobi, ROOM_SEARTH);
		stc("The land returns to its normal climate.\n\r",ch);
		SET_RTIMER(ch->in_room, RTIMER_EARTH, 0);
	        act("$n deftly makes a few handmotions, and the land returns to normal.",ch,NULL,NULL,TO_ROOM);
	}
	else{
		SET_BIT(ch->in_room->shinobi, ROOM_SEARTH);
		SET_RTIMER(ch->in_room, RTIMER_EARTH, 10);
	        ch->in_room->next_update = update_room_list;
	        update_room_list = ch->in_room;
		stc("You turn the land into a barren waste.\n\r",ch);
	        act("$n deftly makes a few handmotions, and the land become a barren waste.",ch,NULL,NULL,TO_ROOM);
		if(IS_SET(ch->in_room->shinobi, ROOM_SWIND)){
			REMOVE_BIT(ch->in_room->shinobi, ROOM_SWIND);
			act("The tornadoes return to the sky.",ch,NULL,NULL,TO_ROOM);
		}
		ch->pcdata->powers[S_TIMER] = 10;
	}


}

void jutsu_shoju(CHAR_DATA *ch, char *argument)
{

	OBJ_DATA *obj;
	int oldlev = ch->level;

        if ( (obj = get_obj_carry(ch, argument, ch)) == NULL)
          {
                 send_to_char("You are not carrying that item.\n\r", ch);
                 return;
          }
        if (IS_SET(obj->quest2,QUEST2_SHINOBI))
          {
                send_to_char("You cant do it twice.\n\r",ch);
                return;
          }
	if(obj->pIndexData->vnum != 406){
		stc("Only on shinobi katana's.\n\r",ch);
		return;
	}
	if(ch->practice < 2000){
		stc("You need 2000 primal to do this.\n\r",ch);
		return;
	}
        if (obj->item_type == ITEM_WEAPON)
          {
                 obj->value[1] += 90;
                 obj->value[2] += 90;
                 obj->level     =  50;
		ch->level = 12;
                 oset_affect(ch,obj,200,APPLY_HITROLL,TRUE);
                 oset_affect(ch,obj,200,APPLY_DAMROLL,TRUE);
                 oset_affect(ch,obj,-200,APPLY_AC,TRUE);
                 obj->condition = 100;
                 obj->toughness = 100;
                 obj->resistance = 1;
                 act("You bless $p with the holy energy of the shinobi.", ch, obj, NULL, TO_CHAR);
                 act("$n blesses $p with the holy energy of the shinobi.", ch, obj, NULL, TO_ROOM);
                 if(!IS_SET(obj->weapflags, WEAPON_FLAMING)) SET_BIT(obj->weapflags, WEAPON_FLAMING);
                 if(!IS_SET(obj->weapflags, WEAPON_FROST)) SET_BIT(obj->weapflags, WEAPON_FROST);
                 if(!IS_SET(obj->weapflags, WEAPON_VAMPIRIC)) SET_BIT(obj->weapflags, WEAPON_VAMPIRIC);
                 if(!IS_SET(obj->weapflags, WEAPON_SHARP)) SET_BIT(obj->weapflags, WEAPON_SHARP);
                 if(!IS_SET(obj->weapflags, WEAPON_SHOCKING)) SET_BIT(obj->weapflags, WEAPON_SHOCKING);
                 if(!IS_SET(obj->weapflags, WEAPON_POISON)) SET_BIT(obj->weapflags, WEAPON_POISON);
		obj->value[0] = 18000;
		ch->level = oldlev;
	         SET_BIT(obj->quest2, QUEST2_SHINOBI);
		 ch->practice -= 2000;
                 WAIT_STATE(ch, 20);
                 return;
          }
	else stc("Only on weapons.\n\r",ch);

}
void jutsu_mian(CHAR_DATA *ch, CHAR_DATA *victim)
{
	int amount;
	if(!IS_CLASS(victim, CLASS_SHINOBI) || (IS_CLASS(victim,CLASS_SHINOBI) && victim->pcdata->powers[S_ELEMENT] == S_SHADOW)){
		stc("You can not focus your energies on them.\n\r",ch);
		return;
	}
	act("$n focus's their holy energy upon $N.",ch,NULL,victim,TO_ROOM);
	act("You focus your holy energy upon $N.",ch,NULL,victim,TO_CHAR);
	act("$n focus's their holy energy upon you.",ch,NULL,victim,TO_VICT);
	amount = ch->pRank * 250;
	amount *= ch->pcdata->powers[S_RANK] + 1;
	amount /= 2;

	victim->hit = UMIN(victim->hit + amount, victim->max_hit);
	victim->mana = UMIN(victim->mana + amount, victim->max_mana);
	victim->move = UMIN(victim->move + amount, victim->max_move);
	regen_limb(victim);
                if (IS_SET(victim->extra,TIED_UP)) REMOVE_BIT(victim->extra,TIED_UP);
                affect_strip(victim,gsn_drowfire);
                affect_strip(victim,gsn_poison);
                affect_strip(victim,gsn_blindness);
                affect_strip(victim,gsn_sleep);
                affect_strip(victim,gsn_curse);
	WAIT_STATE(ch,12);
	return;
	
}
void jutsu_goazento(CHAR_DATA *ch)
{

	int oldlevel = ch->level;

	if(ch->in_room == NULL) return;
	if(ch->pcdata->powers[S_TIMER] > 0){
		stc("You can not make this room safe yet.\n\r",ch);
		return;
	}
	if(IS_SET(ch->in_room->shinobi, ROOM_SSAFE)){
		stc("Removed.\n\r",ch);
	REMOVE_BIT(ch->in_room->shinobi, ROOM_SSAFE);
	SET_RTIMER(ch->in_room, RTIMER_SAFE, 0);
		return;
	}
	SET_BIT(ch->in_room->shinobi, ROOM_SSAFE);
	SET_RTIMER(ch->in_room, RTIMER_SAFE, 20);
	        ch->in_room->next_update = update_room_list;
	        update_room_list = ch->in_room;
	act("A serene feeling comes over the room.\n\r",ch,NULL,NULL,TO_ROOM);
	act("A serene feeling comes over the room.\n\r",ch,NULL,NULL,TO_CHAR);
	ch->level = 12;
	do_peace(ch,"");
	ch->pcdata->powers[S_TIMER] = 5;
	ch->level = oldlevel;	
	WAIT_STATE(ch,30);

}
void jutsu_hishu(CHAR_DATA *ch, CHAR_DATA *tarchar)
{
	char buf[MSL];
	int x;
	int dam;
	int dam2;
	int r;
	dam = ch->pRank * 900;
	dam *= ch->pcdata->powers[S_RANK] + 1;
	dam /= 2;

	if(is_safe(ch,tarchar) == TRUE){
		stc("Not in this room.\n\r",ch);
		return;
	}
	r = number_range(2,6);
	for(x = 1;x < r;x++){
		dam2 = number_range(dam * .6, dam);
		dam = dam2 * .6;
		sprintf(buf,"%s is torn apart by holy power! [#7%d#n]\n\r",GET_PROPER_NAME(tarchar),dam2);
		stc(buf,ch);
		stc(buf,tarchar);
		hurt_person(ch,tarchar,dam2);
		if(tarchar->hit < 1) break;
	}
	set_fighting(ch,tarchar, TRUE);
	WAIT_STATE(ch,12);

}
void jutsu_beoutu(CHAR_DATA *ch)
{

	CHAR_DATA *rch;
	int i, sn, count = 0;
	AFFECT_DATA af;

	for(rch = ch->in_room->people; rch != NULL; rch = rch->next_in_room){
		if(IS_CLASS(rch, CLASS_SHINOBI)) count += 1;
	}

	for(rch = ch->in_room->people; rch != NULL; rch = rch->next_in_room){
		if(IS_CLASS(rch, CLASS_SHINOBI)){
			if(is_affected(rch,skill_lookup("shinobi bless"))) continue;
			for(i = 0; i < count; i++){
 				sn = skill_lookup("shinobi bless");
			      af.type      = sn;
			      af.duration  = 20;
			      af.location  = APPLY_HITROLL;
			      af.modifier  = 250;
			      af.bitvector = 0;
			      affect_to_char(rch, &af);
			      af.type      = sn;
			      af.duration  = 20;
			      af.location  = APPLY_DAMROLL;
			      af.modifier  = 250;
			      af.bitvector = 0;
			      affect_to_char(rch, &af);
			      af.type      = sn;
			      af.duration  = 20;
			      af.location  = APPLY_AC;
			      af.modifier  = -300;
			      af.bitvector = 0;
			      affect_to_char(rch, &af);
			}
		}
	}
	act("$n blesses all the shinobi in the room.\n\r",ch,NULL,NULL,TO_ROOM);
	act("You bless all the shinobi in the room.\n\r",ch,NULL,NULL,TO_CHAR);
	WAIT_STATE(ch, 40);

}
void jutsu_yu(CHAR_DATA *ch)
{

	if(ch->pcdata->powers[S_TIMER] > 0){
		stc("You can not summon holy energies yet.\n\r",ch);
		return;
	}
	if(IS_SET(ch->in_room->shinobi, ROOM_SHOLY)){
		REMOVE_BIT(ch->in_room->shinobi, ROOM_SHOLY);
		stc("The holy energies in the room vanish.\n\r",ch);
		SET_RTIMER(ch->in_room, RTIMER_HOLY, 0);
	        act("$n deftly makes a few handmotions, and the holy energies in the room vanish.",ch,NULL,NULL,TO_ROOM);
	}
	else{
		SET_BIT(ch->in_room->shinobi, ROOM_SHOLY);
		SET_RTIMER(ch->in_room, RTIMER_HOLY, 10);
	        ch->in_room->next_update = update_room_list;
	        update_room_list = ch->in_room;
		stc("You fil the room with holy energies.\n\r",ch);
	        act("$n deftly makes a few handmotions, and the room is filled with holy energies.",ch,NULL,NULL,TO_ROOM);
		ch->pcdata->powers[S_TIMER] = 10;
	}

}

void jutsu_soothe(CHAR_DATA *ch, CHAR_DATA *victim)
{

	int amount;
	if(!IS_CLASS(victim, CLASS_SHINOBI) || (IS_CLASS(victim,CLASS_SHINOBI) && victim->pcdata->powers[S_ELEMENT] == S_FIRE)){
		stc("You can not focus your energies on them.\n\r",ch);
		return;
	}
	act("$n focus's their soothing energy upon $N.",ch,NULL,victim,TO_ROOM);
	act("You focus your soothing energy upon $N.",ch,NULL,victim,TO_CHAR);
	act("$n focus's their soothing energy upon you.",ch,NULL,victim,TO_VICT);
	amount = ch->pRank * 250;
	amount *= ch->pcdata->powers[S_RANK] + 1;
	amount /= 2;

	victim->hit = UMIN(victim->hit + amount, victim->max_hit);
	WAIT_STATE(ch,12);

}
void jutsu_wash(CHAR_DATA *ch, CHAR_DATA *victim)
{

	if(!IS_SET(victim->newbits2,NEW2_WASH)){
		act("$N's limbs are frozen in a wash of energy!",ch,NULL,victim,TO_ROOM);
		act("$N's limbs are frozen in a wash of energy!",ch,NULL,victim,TO_VICT);
		act("$N's limbs are frozen in a wash of energy!",ch,NULL,victim,TO_CHAR);
		SET_BIT(victim->newbits2,NEW2_WASH);
	}
	else{
		act("$N's limbs thaw.",ch,NULL,victim,TO_ROOM);
		act("$N's limbs thaw.",ch,NULL,victim,TO_CHAR);
		act("$N's limbs thaw.",ch,NULL,victim,TO_VICT);
		REMOVE_BIT(victim->newbits2,NEW2_WASH);
	}
	WAIT_STATE(ch,12);

}
void jutsu_fluidity(CHAR_DATA *ch)
{

	if(!IS_SET(ch->pcdata->powers[S_ACTIVE],S_FLUIDITY)){
		stc("Your body moves like water.\n\r",ch);
		act("$n's body flows like water.",ch,NULL,NULL,TO_ROOM);
		SET_BIT(ch->pcdata->powers[S_ACTIVE],S_FLUIDITY);
	}
	else{
		stc("You stop movin so elegantly.\n\r",ch);
		act("$n's body becomes solid once again.",ch,NULL,NULL,TO_ROOM);
		REMOVE_BIT(ch->pcdata->powers[S_ACTIVE],S_FLUIDITY);
	}

}
void jutsu_aqua(CHAR_DATA *ch, CHAR_DATA *tarchar)
{

	char buf[MSL];
	int x;
	int dam;
	int dam2;
	int r;
	dam = ch->pRank * 900;
	dam *= ch->pcdata->powers[S_RANK] + 1;
	dam /= 3;

	if(is_safe(ch,tarchar) == TRUE){
		stc("Not in this room.\n\r",ch);
		return;
	}
	r = number_range(2,5);
	for(x = 1;x < r;x++){
		dam2 = number_range(dam * .6, dam) / x ;
		sprintf(buf,"%s is struck by a pillar of water! [#L%d#n]\n\r",GET_PROPER_NAME(tarchar),dam2);
		stc(buf,ch);
		stc(buf,tarchar);
		hurt_person(ch,tarchar,dam2);
		if(tarchar->hit < 1) break;
	}
	set_fighting(ch,tarchar, TRUE);
	WAIT_STATE(ch,12);

}
void jutsu_viunemto(CHAR_DATA *ch, CHAR_DATA *victim)
{

	if(victim->level > 6){
		stc("No can do!\n\r",ch);
		return;
	}
	if(ch->in_room == victim->in_room){
		stc("They are already in here!\n\r",ch);
		return;
	}
	if(ch->pcdata->powers[S_VTIMER] > 0){
		stc("Not yet.\n\r",ch);
		return;
	}
	ch->viunemto = victim;
	stc("You prepare to ambush your victim...\n\r",ch);
	if(!IS_SET(ch->newbits2,NEW2_VIUNEMTO)) SET_BIT(ch->newbits2,NEW2_VIUNEMTO);
	act("$n turns into a puddle on the ground...",ch,NULL,NULL,TO_ROOM);

}
void jutsu_storm(CHAR_DATA *ch)
{

        if (3054 == ch->in_room->vnum)
        {
         stc("Not at the Altar!\n\r",ch);
         return;
        }

	if(ch->pcdata->powers[S_TIMER] > 0){
		stc("You can not summon a storm yet.\n\r",ch);
		return;
	}
	if(IS_SET(ch->in_room->shinobi, ROOM_SWATER)){
		REMOVE_BIT(ch->in_room->shinobi, ROOM_SWATER);
		stc("You banish the storms.\n\r",ch);
		SET_RTIMER(ch->in_room, RTIMER_WATER, 0);
	        act("$n deftly makes a few handmotions, and the storms vanish.",ch,NULL,NULL,TO_ROOM);
	}
	else{
		SET_BIT(ch->in_room->shinobi, ROOM_SWATER);
		SET_RTIMER(ch->in_room, RTIMER_WATER, 10);
	        ch->in_room->next_update = update_room_list;
	        update_room_list = ch->in_room;
		stc("You summon forth a storm.\n\r",ch);
	        act("$n deftly makes a few handmotions, and powerful storms roll in.",ch,NULL,NULL,TO_ROOM);
		if(IS_SET(ch->in_room->shinobi, ROOM_SFIRE)){
			REMOVE_BIT(ch->in_room->shinobi, ROOM_SFIRE);
			act("The storm cools the lava.",ch,NULL,NULL,TO_ROOM);
		}
		ch->pcdata->powers[S_TIMER] = 12;
	}


}

void jutsu_apprentice(CHAR_DATA *ch, CHAR_DATA *victim)
{
	char buf[MSL];
	int oldlevel;

	if(IS_NPC(victim)){
		stc("Not on mobs.\n\r",ch);
		return;
	}
	if(victim->level <= 2){
		stc("They have to be an avatar.\n\r",ch);
		return;
	}
	if(victim->class != 0){
		stc("They are already classed.\n\r",ch);
		return;
	}
	if(victim->max_hit < 50000 || victim->max_mana < 50000 || victim->max_move < 50000 || victim->practice < 1000 ||
		victim->wpn[0] < 200
||		victim->wpn[1] < 200
||		victim->wpn[2] < 200
||		victim->wpn[3] < 200
||		victim->wpn[4] < 200
||		victim->wpn[5] < 200
||		victim->wpn[6] < 200
||		victim->wpn[7] < 200
||		victim->wpn[8] < 200
||		victim->wpn[9] < 200
||		victim->wpn[10] < 200
||		victim->wpn[11] < 200){
		stc("They do not have the stats required.\n\r",ch);
		return;
	}

	victim->class = CLASS_SHINOBI;
	victim->pcdata->powers[S_ELEMENT] = ch->pcdata->powers[S_ELEMENT];
	ch->pcdata->powers[S_APP] = 2000;
	stc("Ok.\n\r",ch);
	stc("You are now a shinobi!\n\r",ch);
	oldlevel = ch->level;
	ch->level = 13;
	sprintf(buf, "%s 11", victim->pcdata->switchname);
	do_kingset(ch,buf);
	ch->level = oldlevel;

}

void jutsu_tri(CHAR_DATA *ch)
{
	CHAR_DATA *gch;
	int count = 0;

	if(ch->pcdata->powers[S_TIMER] > 0){
		stc("You dont have the power to do this yet.\n\r",ch);
		return;
	}
	for(gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room){
		if(!IS_NPC(gch) && IS_CLASS(gch,CLASS_SHINOBI)) count += 1;
	}

         if (3054 == ch->in_room->vnum)
        {
         stc("Not at the Altar!\n\r",ch);
         return;
        }
/*
        if (3000 <= ch->in_room->vnum && ch->in_room->vnum <= 4000)
        {
	 stc("Not in Midgaard!\n\r",ch);
	 return;
	}
*/
	if(count < 3){
		stc("There are not enough shinobi in the room.\n\r",ch);
		return;
	}
	        ch->in_room->next_update = update_room_list;
	        update_room_list = ch->in_room;
	if(IS_SET(ch->in_room->shinobi, ROOM_SEARTH)){
		REMOVE_BIT(ch->in_room->shinobi, ROOM_SEARTH);
	}
	if(IS_SET(ch->in_room->shinobi, ROOM_SFIRE)){
		REMOVE_BIT(ch->in_room->shinobi, ROOM_SFIRE);
	}
	if(IS_SET(ch->in_room->shinobi, ROOM_SWATER)){
		REMOVE_BIT(ch->in_room->shinobi, ROOM_SWATER);
	}
	if(IS_SET(ch->in_room->shinobi, ROOM_SWIND)){
		REMOVE_BIT(ch->in_room->shinobi, ROOM_SWIND);
	}
	if(IS_SET(ch->in_room->shinobi, ROOM_SHOLY)){
		REMOVE_BIT(ch->in_room->shinobi, ROOM_SHOLY);
	}
		
	if(!IS_SET(ch->in_room->shinobi, ROOM_SEARTH)){
		SET_BIT(ch->in_room->shinobi, ROOM_SEARTH);
		SET_RTIMER(ch->in_room, RTIMER_EARTH, 10);
	}
	if(!IS_SET(ch->in_room->shinobi, ROOM_SFIRE)){
		SET_BIT(ch->in_room->shinobi, ROOM_SFIRE);
		SET_RTIMER(ch->in_room, RTIMER_FIRE, 10);
	}
	if(!IS_SET(ch->in_room->shinobi, ROOM_SWIND)){
		SET_BIT(ch->in_room->shinobi, ROOM_SWIND);
		SET_RTIMER(ch->in_room, RTIMER_WIND, 10);
	}
	if(!IS_SET(ch->in_room->shinobi, ROOM_SHOLY)){
		SET_BIT(ch->in_room->shinobi, ROOM_SHOLY);
		SET_RTIMER(ch->in_room, RTIMER_HOLY, 10);
	}
	if(!IS_SET(ch->in_room->shinobi, ROOM_SWATER)){
		SET_BIT(ch->in_room->shinobi, ROOM_SWATER);
		SET_RTIMER(ch->in_room, RTIMER_WATER, 10);
	}
        act("$n deftly makes a few handmotions, and the shinobi's might wreaks havoc in the room.",ch,NULL,NULL,TO_ROOM);
	ch->pcdata->powers[S_TIMER] = 20;


}

