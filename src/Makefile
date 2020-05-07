CC      = gcc
C_FLAGS = -w -ggdb3 -g -Wall
L_FLAGS = -lz -lcrypt -lpthread
O_DIR   = o/

FILES = act_comm.c act_info.c act_move.c act_obj.c act_wiz.c bit.c mccp.c \
        board.c build.c clan.c comm.c daemon.c db.c angel.c x-socials.c\
        demon.c drow.c fight.c kav_wiz.c handler.c kav_fight.c kav_info.c \
        xrakisis.c interp.c mage.c magic.c mem.c monk.c monk2.c ninja.c olc.c \
        olc_act.c olc_save.c save.c special.c string.c arena.c auction.c\
        update.c vamp.c ww.c shapeshifter.c tanarri.c undead_knight.c \
        samurai.c wizutil.c const.c cyborg.c lich.c jope.c religion.c \
        jobo_king.c jobo_fight.c jobo_act.c jobo_wiz.c jobo_util.c \
        jobo_comm.c jobo_save.c social-edit.c jobo_update.c spiderdroid.c \
        jobo_quest.c msp.c kav_soundex.c desc_map.c jobo_shop.c zarius_proj.c \
        zarius.c changes.c vote.c skyblade.c ghoul.c draconian.c thief.c \
        shadow.c priest.c jedi.c psycho.c giant.c hobbit.c fae.c drone.c \
        zarius_quest.c zarius_bank.c olc_mpcode.c mob_prog.c weather.c \
        mob_cmds.c recycle.c wizlist.c zarius_comm.c zarius_games.c mudmsg.c \
        zarius_shop.c md5.c resistance.c music.c hutoshi.c class.c \
        akurei.c elemental.c drexx.c paladin.c blade.c dragon.c zombie.c shinobi.c \
	races.c ashur.c

     
O_FILES = $(patsubst %.c,$(O_DIR)%.o, $(FILES))

merc: $(O_FILES)
	rm -f mcloud
	$(CC)  -o mcloud $(O_FILES) $(L_FLAGS)
	chmod g+w mcloud
	chmod g+w $(O_FILES)

$(O_DIR)%.o: %.c
	$(CC) -c $(C_FLAGS) $< -o $@

clean:
	rm -f $(O_FILES) ; rm -f mcloud

.c.o: merc.h
	$(CC) -c $(C_FLAGS) $<
