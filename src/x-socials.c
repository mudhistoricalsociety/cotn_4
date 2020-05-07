/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc       *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                                                                         *
 *  Dystopia Mud improvements copyright (C) 2000, 2001 by Brian Graversen  *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/
 /***************************************************************************
 *                                 _/                            _/        *
 *      _/_/_/  _/_/      _/_/_/  _/    _/_/    _/    _/    _/_/_/         *
 *     _/    _/    _/  _/        _/  _/    _/  _/    _/  _/    _/          *
 *    _/    _/    _/  _/        _/  _/    _/  _/    _/  _/    _/           *
 *   _/    _/    _/    _/_/_/  _/    _/_/      _/_/_/    _/_/_/            *
 ***************************************************************************
 * Mindcloud Copyright 2001-2003 by Jeff Boschee (Zarius),                 *
 * Additional credits are in the help file CODECREDITS                     *
 * All Rights Reserved.                                                    *
 ***************************************************************************/

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"

/*
 * The X-social table.
 * Add new X-socials here.
 * Alphabetical order is not required.
 */
const struct xsocial_type xsocial_table[] = {
{

"xDfrontcuff",

"Do this to which sub?", NULL,

"You cuff $N's wrists together infront of $S stomach and straps them above $S head.",

"$N's $r cuffs $S wrists together infront of $S stomach and straps them above $S head.",

"Your $r cuffs your wrists together infront of your stomach and straps them above your head",

"Not on yourself!",

NULL,

0, 0, 0, 1, 1, FALSE

},



{

"xDbackcuff",

"Do this to which sub?", NULL,

"You cuff $N's wrists together behind $N back rendering them helpless.",

"$n cuffs $N's wrists together behind $N's back rendering them helpless.",

"Your $r cuffs your wrists together behind your back rendering you helpless before $m.",

"Not on yourself!",

NULL,

0, 0, 0, 1, 1, FALSE

},



{

"xSgreet",

"Do this to which sub?", NULL,

"You kneel before your $R in greeting and loving kiss $S feet, awaiting their command.",

"$n kneels before $s $R in greeting and loving kisses their feet, awaiting their command.",

"Your slave kneels before you in greeting, lovingly kissing your feet awaiting your command.",

"Not on yourself!",

NULL,

0, 0, 0, 1, 1, FALSE

},





{

"xstrawberry",

"Eh?",

NULL,

"You seductively dip a ripe red strawberry in whip cream before placing it to $N lips smiling as they take a bite.",

"$n places a strawberry in whip cream then slowly places it to $N's lips",

"$n Slowly and seductively dips a ripe red strawberry in whipcream before gently placing it on your lips to bite smiling the entire time.",

"Why not just eat it normally?",

NULL,

0, 0, 0, 1, 1, FALSE

},



{

"xsfingers",

"suck on who's fingers?",

NULL,

"You give $N a dark, sexy look and slowly start licking and sucking on $S fingers.",

"$n gives $N a dark, sexy look and slowly starts licking and sucking on $N's fingers. Meow.",

"$n gives you a dark, sexy look and slowly starts licking and sucking on your fingers. MMMmm.",

"Bah. Find someone elses fingers to suck on.",

NULL,

0, 0, 0, 5, 5, FALSE

},



{

"xtickle",

"tickle WHO?",

NULL,

"You glide your fingers slowly around $N's neck making your way down to the small of $S back then slightly tickling as you run your fingers along their waist line. While kissing $M lovingly.",

"$n slowly runs $s hands down $N's back while kissing $M lovingly",

"$n takes $s fingers and slowly tickles your neck then moves down to the small of your back making feather touches that drive you wild. They kiss you passionately and you know how much they love you.",

"You cant tickle yourself moron",

NULL,

0, 0, 0, 1, 1, FALSE

},



{

"xsnuggle",

"With whom?",

NULL,

"You snuggle close to $N running your hands through $S hair and kissing $M tenderly on the lips",

"$n snuggles close to $N running $s hands through $S hair and kisses $M tenderly on the lips.",

"$n snuggles close to you running $s fingers through your hair and kisses you tenderly on the lips.",

NULL,

NULL,

0, 0, 0, 5, 7, FALSE

},



{

"xflick",

"On whom do you with to do this?",

NULL,

"You mischievously flick your tongue at $N's pubic area, its tip teasing $S groin.  Heat rises from the area, suffusing a warm glow onto your face.",

"$n mischievously flicks $s tongue at $N's pubic area, its tip teasing $S groin.  You notice that $N tenses slightly in response.",

"$n mischievously flicks $s tongue at your pubic area, its tip teasing your groin.  Your body tenses slightly in reflex, your arousal growing.",

NULL,

NULL,

0, 0, 0, 3, 8, FALSE

},



{

"xwhimper",

"Whimper at whom?",

NULL,

"You whimper softly with pleasure.",

"$n whimpers softly with pleasure.",

"$n whimpers softly with pleasure.",

"No.",

NULL,

0, 0, 0, 1, 1, FALSE

},



{

"xtongue",

"On whom do you wish to do this?",

NULL,

"You run your tongue all over $N's body, taking special care to probe deeper where $S is most sensitive.",

"$n runs $s tongue all over $N's body, probing deeper in $S most sensitive parts.",

"$n runs $s tongue all over your body, probing deeper where you are most sensitive.  You feel your temperature rise.",

NULL,

NULL,

0, 0, 0, 2, 6, FALSE

},



{

"xbite",

"Umm pick the person you wish to do this to ya moron!",

NULL,

"You gently bite down on $N's neck, taking a fold of skin between your teeth, sucking softly while biting a little harder, mixing pain with pleasure.",

"$n bites down on $N's neck, twisting a fold of skin between their teeth, causing $N to whimper loudly.",

"You twist with pain, and groan with pleasure as $n bites down on your neck, suckling softly and biting down.",

"NOT.",

NULL,

0, 0, 0, 12, 15, FALSE

},



{

"xabite",

"Bite whom?",

NULL,

"You bend $N over with a gleam in your eye and bite down on $S ass, then you slowly lick the red mark as your hands caress $M.",

"$n bends $N over and bites down on $S ass.",

"You gasp as $n bends you over and bites down on your ass, then gently runs $s tongue over the red mark as $s slowly caresses you.",

"No.",

NULL,

0, 0, 0, 10, 10, FALSE

},



{

"xtalk",

"Tell this to whom?",

NULL,

"You whisper in $N's ear, explaining just how you intend to fuck $S brains out.",

"$n whispers in $N's ear, explaining just how $e intends to fuck $S brains out.",

"$n whispers in your ear, explaining just how $e intends to fuck your brains out.",

NULL,

NULL,

0, 0, 0, 10, 20, FALSE

},





{

"xearlobe",

"On whom do you wish to do this?",

NULL,

"You gently tug on $M earlobe with your teeth.",

"$n gently tugs on $N's earlobe with $s teeth.",

"$n gently tugs on your earlobe with $s teeth.",

"Not on yourself!",

NULL,

0, 0, 0, 1, 1, FALSE

},



{

"xkneel",

"Drop to your knees before who?",

NULL,

"You drop to your knees before $M, begging for mercy.",

"$n drops to $s knees before $N, begging for $M mercy.",

"$n drops to $s knees before you, begging for mercy.",

"Nope.",

NULL,

0, 0, 0, 7, 7, FALSE

},



{

"xcnipples",

"On whom do you wish to do this?",

NULL,

"You put some clamps on $N's nipples, and the effect they have is obvious.",

"$n puts some nipple clamps on $N, with immediate effect.",

"$n puts some clamps on your nipples, and you gasp. The pain is exquisite, but after the initial bite, a deliciously warm glow begins to come off them.",

"Are you serious?",

NULL,

0, 0, 0, 5, 15, FALSE

},



{

"xtoes",

"On whom do you wish to do this?",

NULL,

"You begin slowly sucking on $N toes, giving special attention to each one.",

"$n begins to slowly suck on $N toes, giving special attention to each one.",

"$n begins to slowly suck on your toes, giving special attention to each one.",

"Are you kidding?",

NULL,

0, 0, 0, 5, 10, FALSE

},



{

"xlove",

"On whom do you wish to do this?",

NULL,

"As you look into $N's eyes you whisper, I love you with all my heart.",

"$n looks into $N’s eyes, whispering I love you with all my heart.",

"$n looks into your eyes, whispering I love you with all my heart.",

"No.",

NULL,

0, 0, 0, 5, 5, FALSE

},



{

"xfrench",

"On whom do you wish to do this?",

NULL,

"You give $M a deep throbbing kiss, rolling your tongue around $S.",

"$n gives $N a deep throbbing kiss.",

"$n gives you a deep throbbing kiss, rolling $s tongue around yours.",

"Not on yourself!",

NULL,

0, 0, 0, 1, 1, FALSE

},



{

"xkissneck",

"On whom do you wish to do this?",

NULL,

"You slowly and softly kiss and nuzzle $M neck.",

"$n slowly and softly kisses and nuzzles $N's neck.",

"$n slowly and softly kisses and nuzzles your neck.",

"Not on yourself!",

NULL,

0, 0, 0, 1, 1, FALSE

},



{

"xwcream",

"On whom do you wish to do this?",

NULL,

"You spray whipped cream all over $M body, licking it up with your tongue.",

"$n coats $N's body in whipped cream and then licks it up with their tongue.",

"$n covers your body in whipped cream, and you moan as it gets licked up with their tongue.",

"No.",

NULL,

0, 0, 0, 5, 5, FALSE

},

{

"xice",

"On who?",

NULL,

"You run a cold piece of ice down $M back, following up with your tongue.",

"$n runs a piece of ice over $N's back, following up with their tongue.",

"$n runs a piece of ice over your burning hot flesh, following up with their tongue.",

"No.",

NULL,

0, 0, 0, 6, 8, FALSE

},



{

"xwhip",

"Who's been bad?",

NULL,

"You smirk slightly as you crack a whip over $N's tender skin, making $M whimper.",

"$n grins slightly as he whips $N's tender flesh, making $M whimper.",

"$n grins wickedly as they whip your tender flesh, making you whimper slightly in fear.",

"No.",

NULL,

0, 0, 0, 9, 2, FALSE

},

{

"x-moan",

"On whom do you wish to do this?",

NULL,

"You start moaning Oh $N...oh yes...don't stop...mmMMmm...",

"$n starts moaning Oh $N...oh yes...don't stop...mmMMmm...",

"$n starts moaning Oh $N...oh yes...don't stop...mmMMmm...",

"Not on yourself!",

0, 2, 0, 1, 1, FALSE

},



{

"xnuttella",

"On whom do you wish to do this?",

NULL,

"You cover $N's naked flesh with a popular choc spread, before licking it off.",

"$n smears a chocolate spread over $N's body, licking $M clean with relish",

"$n smears a popular choc spread on your naked flesh, licking it off you",

"Not on yourself!",

NULL,

0, 0, 0, 5, 20, FALSE

},



{

"xstroke",

"On whom do you wish to do this?",

NULL,

"You lightly run your fingers along the insides of $S thighs.",

"$n lightly runs $s fingers along the insides of $N's thighs.",

"$n lightly runs $s fingers along the insides of your thighs.",

"Not on yourself!",

NULL,

0, 0, 0, 5, 10, FALSE

},

{

"xtease",

"Who's licking who?",

NULL,

"You slowly kiss and lick $S body using your tongue to slowly navigate.",

"$n slowly kisses upon $N's naked body, using $s tongue as a erotic tool.",

"$n slowly kisses you down your body, feeling with $s tongue where he wants his mouth to go.",

"To hard to think right now.",

NULL,

0, 0, 0, 5, 10, FALSE

},

{

"xsoftmoan",

"You moan at who?",

NULL,

"You moan softly with soft words of pleasure,",

"$n softly moans in $N's ear with an erotic passion.",

"$n softly moans in your ear with an erotic passion.",

"Don't think, just love.",

NULL,

0, 0, 0, 10, 10, FALSE

},

{

"xscream",

"You what?",

NULL,

"You scream ecstasy as you desire so bad for $N to make you cum.",

"$n says 'Don't stop baby. oooOOOooo you make me feel so good. $n begs $N to make $m cum.'",

"Don't stop baby. oooOOOooo aaah, you make my body feel good. $n desires for you to make $m cum.",



"Feel's too good right now.",

NULL,

0, 0, 0, 5, 10, FALSE

},

{

"xchocolate",

"Do this to who?",

NULL,

"You slowly rub chocolate syrup on $S private area. Licking it slowly as the syrup starts to run.",

"$n slowly rubs chocolate syrup on your body. Licking it off til $e finds that spot, driving you wild.",

"$n slowly rubs chocolate syrup on $N's private area. Licking it off slowly as the syrup starts to run down $N's special area.",

"What?",

NULL,

0, 0, 0, 5, 10, FALSE

},



{

"xtender",

"On whom do you wish to do this?",

NULL,

"You run your fingers through $S hair and kiss $M tenderly on the lips.",

"$n runs $s fingers through $N's hair and kisses $M tenderly on the lips.",

"$n runs $s fingers through your hair and kisses you tenderly on the lips.",

"Not on yourself!",

NULL,

0, 0, 0, 1, 1, FALSE

},



{

"xtie",

"On whom do you wish to do this?",

NULL,

"You tie $N to the bed posts!",

"$n ties $N's body, fixing $M helplessly on the bed.  Oh!! Kinky!",

"$n ties you to the bed post, leaving you completely at $s mercy",

"Not on yourself!",

NULL,

0, 0, 0, 15, 10, FALSE

},



{

"xgag",

"Gag whom?",

NULL,

"You tear off a small strip of $N's clothing, and stuff it into their mouth, tying it back behind their head to secure it.",

"$n reaches down and tears off a small strip of $N's clothing, stuffing it into $S mouth and tying it behind $S head, effectively gagging $S.",

"$n tears off a strip of your clothing, and as you look up at $s, $s shoves it into your mouth and ties it behind your head, gagging you.",

"No.",

NULL,

0, 0, 0, 5, 2, FALSE

},



{

"xblindfold",

"Blindfold who?",

NULL,

"You take out a black silk blindfold, and place it over $N's eyes, tying it securely.",

"$n takes out a black silk blindfold, and places it over $N's eyes, tying it securely behind $S.",

"$n takes out a black silk blindfold, and grins as $s places it over your eyes, tying it behind your head.",

"No.",

NULL,

0, 0, 0, 7, 2, FALSE

},



{

"xknife",

"On whom do you wish to do this?",

NULL,

"You lightly run a cool knife over $N's skin, letting it slide across their body and make weak cuts in their flesh.",

"$n lightly traces a knife over $N's skin, smiling as it makes a few shallow cuts, drawing a bit of blood.",

"$n lightly runs a knife over your skin, and you shiver a bit as it makes a few shallow cuts, drawing a bit of your blood.",

"No.",

NULL,

0, 0, 0, 15, 3, FALSE

},



{

"xwithdraw",

"On whom do you wish to do this?",

NULL,

"You gently pull yourself free of $M.",

"$n gently pulls $mself free of $N.",

"$n gently pulls $mself free of you.",

"Not on yourself!",

NULL,

0, 1, 0, 0, 0, FALSE

},



{

"xlvib",

"On who?",

NULL,

"You produce a thick pink vibrator, and you step behind $N with it, grinning a bit as you run it up and down her pussy lips.  After a moment you push it in and twist it, starting to slowly pump it in and out.",

"$n produces a thick pink vibrator, and walks behind $N, holding it in her hands.  She grins a bit as she pushes it to the girl's wet pussy lips, and pushes it in, twisting it as she begins to pump it in and out.",

"$n produces a thick pink vibrator, and steps behind you as you whimper a bit.  After a moment, you feel it running up and down your cunt lips, and you let out a moan as it slips inside you, twisting as it slides in and out.",

"No.",

NULL,

3, 0, 250, 17, 39, FALSE

},

{

"xlafinger",

"Use this on who?",

NULL,

"You take your finger to $N's mouth and have her lick it before taking it to her puckered ass and slowly slipping it in.",

"$N sucks lightly on $n's finger a moment before she takes it between $N's ass cheeks and pushes it in to the knuckle, twisting.",

"$n brings her finger to your lips, and you suck on it a bit before she pulls it away.  A moment later, she presses it to your tight little ass and pushes it in, twisting as you whimper softly.",

"No.",

NULL,

3, 0, 0, 20, 25, FALSE

},



{

"xlfinger",

"On whom do you wish to do this?",

NULL,

"You spread $N's thighs and slip your finger between them, tracing it over her lips a few times before pushing it past and into her hot and wet pink folds.",

"$n spreads $N's thighs and plunges her finger into her twat, pumping it out a few times as sweet, sticky juice spills over it.",

"$n spreads your legs and begins to trace her finger over your moistened lips, and you let out a soft moan as she slips it past them and into your hot snatch, plunging it in and out as it collects your juices.",

"No.",

NULL,

3, 0, 250, 19, 30, FALSE

},



{

"xlgag",

"Gag who?",

NULL,

"You pick up your soaked panties, and wad them up into a ball, stuffing them into $N's mouth to gag her.",

"$n picks up her drenched panties, wadding them into a ball and stuffing them into $N's mouth, gagging her.",

"$n picks up her panties, soaked through with her cunt juice, and wads them into a ball, stuffing them into your mouth to gag you.",

"No.",

NULL,

3, 0, 0, 7, 9, FALSE

},



{

"xlstroke",

"On whom do you wish to do this?",

NULL,

"You slip your finger between $N's legs and gently stroke her throbbing clit, making her squirm and moan.",

"$n slips her finger between $N's damp thighs and begins to softly stroke her clit, making the poor girl squirm and moan in pleasure.",

"$n slips her finger between your thighs and begins to softly stroke your throbbing clit, making you shut your eyes and moan in pleasure.",

"No.",

NULL,

3, 0, 250, 10, 17, FALSE

},



{

"xlnibble",

"On whom do you wish to do this?",

NULL,

"You lean to nibble on $N's shoulder, working your way down to her hard nipples, giving them a tug with your teeth.",

"$n leans to nibble on $N's shoulder, working her way down to $N's nipples, tugging on them with her teeth.",

"$n leans to nipple on your shoulder, working her way down to your nipples, tugging on them with her teeth.",

"No.",

NULL,

3, 0, 250, 10, 17, FALSE

},



{

"xlundress",

"On whom do you wish to do this?",

NULL,

"You reach back and unhook your bra, letting it slide to the floor with your panties.",

"$n reaches back and unhooks her bra, letting it fall to the floor along with her panties.",

"$n reaches back and unhooks her bra, letting it fall to the floor along with her panties.",

"Pick a valid target.",

NULL,

3, 0, 0, 15, 15, FALSE

},



{

"xllick",

"On whom do you wish to do this?",

NULL,

"You bury your face between $N's thighs, letting your tongue slide over her swollen pussy lips and gently tease her clit.",

"$n pushes $N to the floor and buries her face between her thighs, letting her tongue slide over $N's pussy lips and caress her clit.",

"$n pushes you to the floor, burying her face between your damp thighs as she lets her tongue caress your swollen pussy lips, and you squirm as it lightly runs over your clit.",

"No.",

NULL,

3, 0, 250, 25, 37, FALSE

},



{

"xl69",

"On whom do you wish to do this?",

NULL,

"You lie down on your back and motion for $N to straddle your face, moaning as she licks at you while you hungrily devour her wet snatch.",

"$N lowers her pussy over $n's face and places her face between her thighs, both of them gasping as their tongues dart in and out of each other.",

"$n pulls you on top of her, placing your hot, wet pussy right over her face, and you do the same, both of you panting and moaning as you devour each other.",

"No.",

NULL,

3, 1, 0, 40, 40, FALSE

},



{

"xlbreasts",

"On whom do you wish to do this?",

NULL,

"You lean forward and place your hands over $N's supple breasts, kneading them softly as she moans.",

"$n places her hands over $N's soft breasts and begins to gently knead them, smiling as she begins to moan.",

"$n places her hands over your soft breasts and begins to knead them, a smile playing over her face as you start moaning.",

"No.",

NULL,

3, 0, 0, 10, 18, FALSE

},



{

"xfDgate",

"This doesn't work right", NULL,

"You strap the Gates of Hell on your slave's cock, causing $M to get aroused.",

"$n straps the Gates of Hell on $N's cock, causing $M to get aroused.",

"Your $r straps the Gates of Hell on your cock, causing you to harden from the pressure",

"Not on yourself!",

NULL,

2, 0, 0, 5, 5, FALSE

},



{

"xflapdance",

"To whom?",

NULL,

"You straddle $N's lap while wiggling around and slowly grinding up and down dancing seductively in front of them.",

"$n climbs on $N's lap and begins a very sensual dance.",

"$n begins to dance seductively on your lap making you squirm and moan slightly from the pleasure.",

"No.",

NULL,

2, 0, 0, 9, 14, FALSE

},

{

"xfsuck",

"To whom?",

NULL,

"You slowly take $N's penis in your hands. Entering it slowly in your mouth, softly sucking and rubbing your tounge at the top of his cock.",

"$n slowly takes $N's penis in her hands. Putting it slowly in her mouth as $s sofly sucks and licks on $N's dick.",

"$n begins to take your penis in her hands, putting it in her mouth and softly sucking and licking on it.",

"You don't have a dick.",

NULL,

2, 0, 0, 20, 35, FALSE

},

{

"xfbtease",

"On whom do you wish to do this?",

NULL,

"You sensually lick $N's glans and shaft with your tongue, never actually allowing $M into your mouth even though $E's dying to be there.",

"$n sensually licks around $N's cock, teasing $M with the prospect of a blowjob but not quite fulfilling.",

"$n sensually licks your glans and shaft with $s tongue, never actually allowing you into $s mouth even though you're dying to be there.",

"No.",

NULL,

2, 0, 0, 5, 14, FALSE

},



{

"xfslide",

"On whom do you wish to do this?",

NULL,

"You gently slide up and down on $N's erect penis, your hands braced on his chest.",

"$n gently slides up and down on $N's erect penis, her hands braced on his chest.",

"$n gently slides up and down on your erect penis, her hands braced on your chest.",

"No.",

NULL,

2, 2, 0, 12, 12, TRUE

},



{

"xfride",

"On whom do you wish to do this?",

NULL,

"You ride $N , moving slowly up and down on his hard cock.",

"$n starts riding $N in wild abandon.",

"$n rides your hard cock sending waves of pleasure over you.",

"No.",

NULL,

2, 2, 0, 9, 14, TRUE

},



{

"xfwiggle",

"On whom?",

NULL,

"You wiggle your groin playfully, causing $N to groan loudly as his penis is rubbed against your vaginal walls.",

"$n wiggles her groin playfully, and $N groans in response as his erect penis is moved around inside her vagina.",

"$n wiggles her groin playfully, and you groan loudly as your erect penis is rubbed around inside her hot, slick vagina.",

NULL,

NULL,

2, 2, 0, 6, 12, TRUE

},



{

"xfmassage",

"On whom do you wish to do this?",

NULL,

"You take $N's penis and scrotum into your hands, and then gently knead and press them into arousal.",

"$n takes $N's penis and scrotum into her hands, and then gently kneads and presses them.  You can see his penis begin to enlarge within $s tender grasp.",

"$n takes your penis and scrotum into her hands, and then gently kneads and presses them.  You get a warm feeling in your groin as it slowly expands.",

"No.",

NULL,

2, 0, 0, 1, 10, FALSE

},



{

"xfbspank",

"On whom do you wish to do this?",

NULL,

"You grasp $N's balls and lightly spank them, letting him feel the sting of pain along with the pleasure.",

"$n grasps $N's balls and starts spanking them, turning them a nice shade of pink.",

"$n grasps your balls and starts spanking them. You feel the slight sting of pain along with the pleasure.",

"No.",

NULL,

2, 0, 0, 1, 20, FALSE

},



{

"xfamount",

"On whom do you wish to do this?",

NULL,

"You push $N onto $S back, and quickly coat $S cock with pussy juice. Kneeling up, you sit your ass on $S cock, gently grinding and wiggling your hips until you gasp as $N sinks deep inside your most private place.",

"$n pushes $N onto $S back, and quickly coats $S cock with $s pussy juice. Kneeling up, $e sits $s ass on $S cock, gently grinding and wiggling $s hips until $e gasps as $N sinks deep inside her.",

"$n pushes you onto your back, and quickly coats your cock with $s pussy juice. Kneeling up, $e sits $s ass on your cock, gently grinding and wiggling $s hips until $e gasps as you sink deep inside her.",

NULL,

NULL,

2, 1, 250, 10, 15, FALSE

},



{

"xforal",

"On whom do you wish to do this?",

NULL,

"You take $N's hot member in your hands, licking his cock from base to tip.",

"$n takes $N's penis in $s hands, licking $N's cock from base to tip.",

"You let out a small moan as $n takes your cock into her hands, licking it from your balls to the tip.",

"On yourself? I'm impressed!",

NULL,

2, 0, 0, 5, 15, FALSE

},



{

"xfspank",

"Who's been naughty?",

NULL,

"You take $N over your knee and spank him hard, making him squirm.",

"$n bends $N over her knees and spanks him hard on the ass, making him squirm.",

"You whimper softly as $n takes you over her knee, spanking you hard on the ass, making it sting.",

"Nah.",

NULL,

2, 0, 0, 25, 4, FALSE

},



{

"xfbeg",

"Beg who, you stupid slut?",

NULL,

"You drop to your kneees before $N, begging for him to make love to you.",

"$n drops to her knees and begs for $N to take her and use her for his pleasure.",

"$n drops to her knees before you, begging for you to make love to her.",

"It doesn't work that way, silly.",

NULL,

2, 0, 0, -10, 12, FALSE

},



{

"xfblowjob",

"On whom do you wish to do this?",

NULL,

"You take $N's hot member in your mouth, sucking $S shaft.",

"$n takes $N's throbbing penis in $s mouth, sucking $N's cock.",

"You gasp as $n takes your penis in $s mouth, licking your head.",

"On yourself? I'm impressed!",

NULL,

2, 1, 0, 10, 25, FALSE

},



{

"xftip",

"On whom do you wish to do this?",

NULL,

"You gently run the tip of your tongue along the underside of $N's hot cock.",

"$n runs the tip of $s tongue along the bottom of $N's pulsing cock.",

"You let out a low moan as $n runs the tip of her tongue along the underside of your cock.",

"I think not.",

NULL,

2, 0, 0, 5, 59, FALSE

},



{

"xfhandjob",

"On who?",

NULL,

"You take his hot cock in your hand and wrap your fingers around it, slowly pumping up and down.",

"$n takes $N's hot cock in her hands, wrapping her fingers around it as she slowly pumps up and down.",

"$n takes your hot cock into her hands, and you moan softly as she wraps her fingers around it, slowly pumping up and down.",

"No.",

NULL,

2, 0, 0, 9, 14, FALSE

},



{

"xfsitface",

"Who gets to taste your sweet juice?",

NULL,

"You push $N onto his back and crawl up his body, sitting down on his face as he licks at your pussy.",

"$n pushes $N onto his back and straddles his face, moaning as he licks her hot pussy.",

"$n pushes you onto your back and straddles your face, moaning and squirming as you eagerly lap at her hot, wet cunt.",

"No.",

NULL,

2, 1, 0, 15, 0, FALSE

},



{

"xf69",

"Who?",

NULL,

"You crawl on top of $N and takes his hot cock between your lips, moaning as he laps at your cunt.",

"$n pushes $N onto his back and crawls on top of him, sucking his cock while he wriggles his tongue about in her cunt.",

"$n pushes you onto your back, crawling on top of you as she sucks on your cock, moaning while you lick her pussy.",

"No.",

NULL,

2, 1, 0, 20, 20, FALSE

},



{

"xfbreasts",

"On whom do you wish to do this?",

NULL,

"You take $S hands and press them to your breasts.",

"$n takes $N's hands and presses them to $m breasts.",

"$n takes your hands and presses them to $m breasts.",

"Not on yourself!",

NULL,

2, 0, 0, 5, 10, FALSE

},



{

"xfrub",

"Who gets to feel the magic touch?",

NULL,

"You gently run your fingers over $N's hardening cock.",

"$n gently runs her fingers over $N's hardening cock.",

"$n gently runs her fingers over your hardening cock.",

"You don't have a penis.  At least, I hope not.",

NULL,

2, 0, 0, 15, 20, FALSE

},

{

"xfcontract",

"On whom do you wish to do this?",

NULL,

"You contract your vaginal muscles, driving $M wild.",

"$n contracts $m vaginal muscles, driving $N wild.",

"$n contracts $m vaginal muscles, driving you wild.",

"Not on yourself!",

NULL,

2, 2, 0, 10, 15, TRUE

},



{

"xfafinger",

"Use this on who?",

NULL,

"You lick your finger and press it to $N's ass, pushing lightly until it pops in and he lets out a small moan.",

"$n licks her finger and takes it to $N's ass, popping it in and gently pumping it back and forth, making him moan.",

"$n licks her finger and takes it to your ass, pushing it in and pumping it in and out, making your dick rock-hard as you moan.",

"No.",

NULL,

2, 1, 0, 20, 23, FALSE

},



{

"xffinger",

"On whom do you wish to do this?",

NULL,

"You put your hands between your legs and begin to masterbate for $N.",

"$n puts $m hands between $m legs and begins to masterbate for $N.",

"$n puts $m hands between $m legs and begins to masterbate for your viewing pleasure.  What a turn on!.",

"Not on yourself!",

NULL,

2, 0, 0, 20, 10, FALSE

},

{

"xffloor",

"On whom do you wish to do this?",

NULL,

"You lie on your back, and pull $M between your parted legs.",

"$n lies on $m back, and pulls $N between $m parted legs.",

"$n lies on $m back, and pulls you between $m parted legs.",

"Not on yourself!",

NULL,

2, 1, 250, 25, 20, TRUE

},



{

"xfgrind",

"On whom do you wish to do this?",

NULL,

"You grind your hips up to meet $S thrusts.",

"$n grinds $m hips up to meet $N's thrusts.",

"$n grinds $m hips up to meet your thrusts.",

"Not on yourself!",

NULL,

2, 2, 0, 15, 10, TRUE

},



{

"xfmount",

"On whom do you wish to do this?",

NULL,

"You push $M onto $S back, and slowly lower yourself onto $S erection.",

"$n pushes $N onto $S back, and slowly lowers $mself onto $S erection.",

"$n pushes you onto your back, and slowly lowers $mself onto your erection.",

"Not on yourself!",

NULL,

2, 1, 250, 25, 20, TRUE

},



{

"xfnails",

"On whom do you wish to do this?",

NULL,

"You scratch your nails down $S back.",

"$n scratches $m nails down $N's back.",

"$n scratches $m nails down your back.",

"Not on yourself!",

NULL,

2, 2, 0, 5, 1, TRUE

},



{

"xfpull",

"On whom do you wish to do this?",

NULL,

"You wrap your arms and legs around $M and pull $M into you.",

"$n wraps $m arms and legs around $N and pulls $M into $m.",

"$n wraps $m arms and legs around you and pulls you into $m.",

"Not on yourself!",

NULL,

2, 1, 250, 15, 10, TRUE

},



{

"xfsqueeze",

"On whom do you wish to do this?",

NULL,

"Your wrap your legs around $M and squeeze tightly.",

"$n wraps $m legs around $N and squeezes tightly.",

"$n wraps $m legs around you and squeezes tightly.",

"Not on yourself!",

NULL,

2, 2, 0, 15, 10, TRUE

},



{

"xftitfuck",

"Whom do you wish to treat to this?",

NULL,

"You take $N's cock and place it between your breasts, as $N gently thrusts.",

"$n takes $N's penis, places it between $s breasts, letting him thrust between.",

"$n takes your cock, places it between her breasts, letting you thrust between.",

"Not on yourself!",

NULL,

2, 1, 0, 10, 20, FALSE

},



{

"xffondle",

"On whom do you wish to perform this?",

NULL,

"You reach down and gently fondle $N's warm penis.",

"$n takes $N's cock into her hands, fondling it gently.",

"$n takes your warm cock into her hands, fondling you gently.",

"No.",

NULL,

2, 0, 0, 5, 13, FALSE

},



{

"xfbob",

"On whom do you wish to perform this?",

NULL,

"You wrap your lips tight around $N's cock and bob your head back and forth, forcing him into your mouth.",

"$n wraps her lips over $N's cock and bobs her head back and forth, making him fuck her mouth.",

"$n wraps her lips tight around your cock and bobs her head back and forth, pumping your length in and out of her mouth.",

"No.",

NULL,

2, 1, 0, 20, 25, FALSE

},



{

"xfundress",

"On whom do you wish to do this?",

NULL,

"You push $N onto the floor, a gleam in your eyes, and tear $S clothes off.",

"$n pushes $N to the floor, tears $S clothes from $S body.",

"$n pushes you to the floor, grins, and rips your clothes from your body.",

"Not on yourself!",

NULL,

2, 0, 0, 1, 1, FALSE

},



{

"xmdrive",

"On whom do you wish to do this?",

NULL,

"You roll your hips forward and drive your penis deeply into $N's mound, causing her luscious lips to part as you run fully within her.",

"$n rolls his hips forward and drives his penis deeply into $N's mound, parting her luscious labial folds and penetrating inside to the hilt.",

"$n rolls his hips forward and drives his penis straight into your mound.  You feel your labial folds push to the sides as his erection penetrates hard.",

"No.",

NULL,

1, 1, 250, 20, 18, TRUE

},

{

"xmlick",

"To whom?",

NULL,

"You slowly make your way down her body til you reach her clit, licking and kissing it and watching her body go wild.",

"$n slowly makes $s way down $N's body til $e reaches $M clit, licking and kissing $M soft pink clit.",

"$n slowly makes his way upon your clit, licking and kissing your soft pink lips as $E starts to moan.",

"I don't think you have a clit.",

NULL,

0, 0, 0, 0, 0, FALSE

},

{

"xmupthrust",

"On whom do you wish to do this?",

NULL,

"Sliding behind $N, you place your hands on her waist and thrust up into her wet pussy.  She gasps as you penetrate her deeply.",

"$n slides behind $N, and places his hands on her waist.  Raising his penis slightly, he thrusts it between her legs, penetrating her mound instantly.",

"$n slides behind you, and places his hands on your waist.  With a sharp motion, he thrusts up into your vagina, penetrating your ready pussy instantly.",

"Not on yourself!",

NULL,

1, 1, 250, 17, 21, TRUE

},



{

"xmassnasty",

"On whom do you wish to do this?",

NULL,

"You force $N to lie facedown on the floor, with you kneeling between $S spread legs. Without warning, you ram your cock into $N's tight ass, making $M cry out with pain.",

"$n forces $N to lie facedown on the floor, with $m kneeling between $S spread legs. Without warning, $n rams $s cock into $N's ass, making $M cry out with pain.",

"$n forces you to lie facedown on the floor, with $m kneeling between your spread legs. Without warning, $n rams $s cock into your tight ass and you cry out with pain.",

"Not hardly.",

NULL,

1, 1, 250, 25, 5, FALSE

},



{

"xmassnice",

"On whom do you wish to do this?",

NULL,

"You gently make $N kneel before also kneeling down behind $M. Using some of $S juices for lubricant, you gently penetrate $N's ass with your cock.",

"$n makes $N kneel before also kneeling down, behind $M, and gently penetrating $N's ass with $s cock.",

"$n gently makes you kneel then kneels down behind you and gently penetrates your ass with $s cock.",

"No.",

NULL,

1, 1, 250, 20, 15, FALSE

},



{

"xmathrust",

"On whom do you wish to do this?",

NULL,

"You drive your cock hard into $N's hot, tight ass.",

"$n  drives $s cock hard into $N's ass.",

"$n drives $s cock hard into your tight ass.",

"Not on yourself!",

NULL,

1, 1, 250, 15, 10, FALSE

},



{

"xmcclit",

"On whom do you wish to do this?",

NULL,

"You carefully clamp $N's clit, with evidently painful but orgasmic effect.",

"$n carefully clamps $N's clit, with evidently painful but orgasmic effect.",

"$n has just put a clamp on your clit! You can barely think at first for the pain, and then for the orgasm which burns up through you.",

"As if!",

NULL,

1, 0, 0, 10, 100, FALSE

},



{

"xmanal",

"On whom do you wish to do this?",

NULL,

"$N screams in extacy as you come from behind and penetrate $M tight ass from behind.",

"$n comes from behind $N and penetrates $M tight ass from behind.",

"You scream as $n comes from behind and penetrates your tight ass from behind!",

"Not on yourself!",

NULL,

1, 1, 0, 25, 25, FALSE

},



{

"xmspank",

"Who's the naughty whore?",

NULL,

"You grab $N by the hair and throw her over your knees, slapping her hard on the ass.",

"$n visciously throws $N over his knee, spanking her relentlessly on her tight ass.",

"You cry out softly as $n visciously takes you over his knee, spanking you mercilessly.",

"Nah.",

NULL,

1, 0, 0, 33, -14, FALSE

},



{

"xm69",

"Whom do you wish to perform this on?",

NULL,

"You pull $N on top of you and spread her legs, burying your face in her pussy as she sucks on your hard cock.",

"$n pulls $N on top of him and buries his face between her legs, eating her out as she goes down on his cock.",

"$n pulls you on top of him and spreads your legs, burying his face between your legs and sucking on your clit as you takes his cock into your mouth.",

"No.",

NULL,

1, 1, 0, 24, 24, FALSE

},



{

"xmbeg",

"Beg who, you worthless bitchboy?",

NULL,

"You drop to your knees, tears in your eyes, and beg for $N to take you as hers.",

"$n drops to his knees before $N, begging for her to take him as hers.",

"$n drops to his knees before you with tears in his eyes, sobbing as he begs for you to take him as your own personal toy.",

"You worthless slut, you can't even do this right.",

NULL,

1, 0, 0, -15, 30, FALSE

},



{

"xmbreasts",

"On whom do you wish to do this?",

NULL,

"You gently run the tip of your tongue across $M naked breasts.",

"$n gently runs the tip of $s tongue across $N's naked breasts.",

"$n gently runs the tip of $s tongue across your naked breasts.",

"Not on yourself!",

NULL,

1, 0, 0, 5, 10, FALSE

},



{

"xmcup",

"On whom do you wish to do this?",

NULL,

"You cup $N's breasts in your hands, and caress $S nipples.",

"$n cups $N's breasts in $s hands and caress' $S nipples.",

"$n cups your breasts in $s palms and caress' your nipples.",

"ON YOURSELF??",

NULL,

1, 0, 0, 1, 5, FALSE

},



{

"xmdoggy",

"On whom do you wish to do this?",

NULL,

"You roll $M onto all fours and penetrate $M hot flesh from behind.",

"$n rolls $N onto all fours and penetrates $M hot flesh from behind.",

"$n rolls you onto all fours and penetrates your hot flesh from behind.",

"Not on yourself!",

NULL,

1, 1, 250, 25, 15, FALSE

},



{

"xmfinger",

"On whom do you wish to do this?",

NULL,

"You slide your fingers between $M legs, gently stroking $M clitoris.",

"$n slides $s fingers between $N's legs, gently stroking $M clitoris.",

"$n slides $s fingers between your legs, gently stroking your clitoris.",

"Not on yourself!",

NULL,

1, 0, 0, 0, 10, FALSE

},



{

"xmfist",

"On whom do you wish to do this? I hope you asked!",

NULL,

"You make a fist and thrust it up $N's spread wide vagina as she screams with pleasure.",

"$N gasps as $n pushes $s hand up between $S legs into $S vagina.",

"$n spreads your legs wide, and thrusts $s hand up your vagina making you cry out in pleasure.",

"C'est non possible.. I hope...",

NULL,

1, 1, 0, 5, 15, FALSE

},



{

"xmfloor",

"On whom do you wish to do this?",

NULL,

"You lower $M to the floor, and slide your body between $M parted legs.",

"$n lowers $N to the floor, and slides $s body between $M parted legs.",

"$n lowers you to the floor, and slides $s body between your parted legs.",

"Not on yourself!",

NULL,

1, 1, 250, 15, 10, TRUE

},



{

"xmjerkoff",

"On whom do you wish to do this?",

NULL,

"You grab your penis and begin to jerkoff for $N.",

"$n grabs your penis and begins to jerkoff for $N.",

"$n grab his penis and begins to jerkoff for your viewing pleasure.  MMmmmmm!.",

"Not on yourself!",

NULL,

1, 1, 0, 25, 5, FALSE

},



{

"xmnipple",

"On whom do you wish to do this?",

NULL,

"You gently twist $M nipple between your thumb and forefinger.",

"$n gently twists $N's nipple between $s thumb and forefinger.",

"$n gently twists your nipple between $s thumb and forefinger.",

"Not on yourself!",

NULL,

1, 0, 0, 5, 10, FALSE

},



{

"xmoral",



"On whom do you wish to do this?",

NULL,

"$N squirms in delight as you bend down and run your tongue along and into $S vagina.",

"$N squirms ecstatically as $n licks and kisses $S loveslit.",

"$n wriggles $s tongue about between your legs, making you squirm in ecstacy.",

"Biologically impossible I think you'll find!",

NULL,

1, 0, 0, 10, 25, FALSE

},



{

"xmpress",

"On whom do you wish to do this?",

NULL,

"You press $M against the wall, pulling $M legs around your hips.",

"$n presses $N against the wall, pulling $M legs around $s hips.",

"$n presses you against the wall, pulling your legs around $s hips.",

"Not on yourself!",

NULL,

1, 1, 250, 25, 20, TRUE

},



{

"xmpull",

"On whom do you wish to do this?",

NULL,

"You grab $M around the hips and pull $M firmly onto your erection.",

"$n grabs $N around the hips and pull $M firmly onto $s erection.",

"$n grabs you around the hips and pulls you firmly onto $s erection.",

"Not on yourself!",

NULL,

1, 1, 250, 10, 10, TRUE

},



{

"xmspoon",

"On whom do you wish to do this?",

NULL,

"You roll $M onto $S side and penetrate $M hot flesh from behind.",

"$n rolls $N onto $S side and penetrates $M hot flesh from behind.",

"$n rolls you onto your side and penetrates your hot flesh from behind.",

"Not on yourself!",

NULL,

1, 1, 250, 20, 20, TRUE

},



{

"xmsuck",

"On whom do you wish to do this?",

NULL,

"You suck slowly on $M nipple, feeling it harden between your lips.",

"$n suck slowly on $N's nipple.",

"$n sucks slowly on your nipple, and you feel it harden between $s lips.",

"Not on yourself!",

NULL,

1, 0, 0, 5, 10, FALSE

},



{

"xmthrust",

"On whom do you wish to do this?",

NULL,

"You thrust deeply between $M warm, slippery thighs.",

"$n thrusts deeply between $N's warm, slippery thighs.",

"$n thrusts deeply between your warm, slippery thighs.",

"Not on yourself!",

NULL,

1, 2, 0, 15, 10, TRUE

},



{

"xmhump",

"On whom do you wish to do this?",

NULL,

"You push $M onto all fours and mount her, madly shoving your hard cock in and out of her tiny hole.",

"$n mounts $N like she was a bitch in heat, madly pumping in and out of her hole.",

"$n pushes you onto all fours like you were a bitch in heat and mounts you as he rapidly shoves his cock in and out of your slutty hole.",

"No!.",

NULL,

1, 2, 0, 35, 65, TRUE

},



{

"xmmfuck",

"Fuck who?",

NULL,

"You have $N wrap her lips tight around your cock, and you thrust gently, pumping your cock in and out of her warm, wet mouth.",

"$N wraps her lips tight aroudn $n's cock, and he thrusts gently, pumping in and out from between her lips.",

"You wrap your lips tight around $n's cock and he thrusts gently, pumping in and out from between your ruby red lips.",

"No.",

NULL,

1, 1, 0, 12, 20, FALSE

},



{

"xmfeed",

"Feed who?",

NULL,

"You gently stick your fingers in $N's hot cunt and bring them to her lips, making her suck them clean.",

"$n sticks his fingers in $N's twat and then brings them to her lips, making her lick them clean.",

"$n sticks his fingers in your dripping wet pussy and then brings them to your lips, where you suck them clean.",

"No.",

NULL,

1, 1, 0, 5, 10, FALSE

},



{

"xmgag",

"Who gets to taste it?",

NULL,

"You ram your cock deep into $M mouth, making her gag and try to swallow.",

"$n rams his cock deep into $N's throat, making her gag and try to swallow.",

"$n shoves his cock deep into your throat, making you gag and try to swallow.",

"No.",

NULL,

1, 1, 0, 5, 13, FALSE

},

{

"xmtug",

"On whom do you wish to do this?",

NULL,

"You gently tug $M nipple between your teeth.",

"$n gently tugs $N's nipple between $s teeth.",

"$n gently tugs your nipple between $s teeth.",

"Not on yourself!",

NULL,

1, 0, 0, 5, 10, FALSE

},



{

"xmnibble",

"On whom do you wish to do this?",

NULL,

"You push $M onto her back and bury your face in her cunt, nibbling gently on her clit.",

"$n pushes $N onto her back and buries his face between her legs, nibbling gently on her clit.",

"$n pushes you onto your back and buries his face in your womanhood, nibbling gently on your clit.",

"Nope.",

NULL,

1, 1, 0, 10, 45, FALSE

},



{

"xmundress",

"Who do you wish to undress?",

NULL,

"You gently tug at $N's garments with your teeth, until $E stands naked before you.",

"$n gently tugs at $N's clothes with his teeth, until $E stands naked infront of $m.",

"$n gently tugs at your clothing with his teeth, you stand naked before $m.",

"Not on yourself!",

NULL,

1, 0, 0, 1, 1, FALSE

},









{

"",

NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, 0, 0, 0, 0, FALSE

}

};

