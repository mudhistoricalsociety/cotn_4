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

/*
 * Command functions.
 * Defined in act_*.c (mostly).
 */

// Cotn
DECLARE_DO_FUN(do_objsell);
DECLARE_DO_FUN(do_identify);
DECLARE_DO_FUN(do_jail);
DECLARE_DO_FUN(do_warden);
DECLARE_DO_FUN(do_freechar);
DECLARE_DO_FUN(do_raceself);
DECLARE_DO_FUN(do_gainlevel);
DECLARE_DO_FUN(do_rompractice);
DECLARE_DO_FUN(do_prof);
DECLARE_DO_FUN(do_convert);
DECLARE_DO_FUN(do_ascension);

// Race Powers -xrak
DECLARE_DO_FUN(do_racepowers);
DECLARE_DO_FUN(do_thunderhammer);
DECLARE_DO_FUN(do_mistform);
DECLARE_DO_FUN(do_weaponenhance);
DECLARE_DO_FUN(do_armorenhance);
DECLARE_DO_FUN(do_flameblade);
DECLARE_DO_FUN(do_goblingrenade);
DECLARE_DO_FUN(do_airshiptravel);
DECLARE_DO_FUN(do_superberserk);
DECLARE_DO_FUN(do_elveninfra);
DECLARE_DO_FUN(do_songs);
DECLARE_DO_FUN(do_elfsing);
DECLARE_DO_FUN(do_lessons);
DECLARE_DO_FUN(do_tremble);
DECLARE_DO_FUN(do_pirate);
DECLARE_DO_FUN(do_toughskin);


DECLARE_DO_FUN(do_game);
DECLARE_DO_FUN(do_replay);
DECLARE_DO_FUN(do_autostore);
DECLARE_DO_FUN(do_kdeposit);
DECLARE_DO_FUN(do_gemote);
DECLARE_DO_FUN(do_surrender);
DECLARE_DO_FUN(do_balance);
DECLARE_DO_FUN(do_membership);
DECLARE_DO_FUN(do_deposit);
DECLARE_DO_FUN(do_withdraw);
DECLARE_DO_FUN(do_scatter);
DECLARE_DO_FUN(do_loop);
DECLARE_DO_FUN(do_weather);
DECLARE_DO_FUN(do_wset);
DECLARE_DO_FUN(do_autoweather);
DECLARE_DO_FUN(do_wipe);
DECLARE_DO_FUN(do_load);
DECLARE_DO_FUN(do_vload);
DECLARE_DO_FUN(do_split);
DECLARE_DO_FUN(do_speak);
DECLARE_DO_FUN(do_autosplit);
DECLARE_DO_FUN(do_unmaster);
DECLARE_DO_FUN(do_endragnarok);
DECLARE_DO_FUN(do_startragbid);
DECLARE_DO_FUN(do_allpeace);
DECLARE_DO_FUN(do_mobquest);
DECLARE_DO_FUN(do_shoot);
DECLARE_DO_FUN(do_unpreg);
DECLARE_DO_FUN(do_unload);
DECLARE_DO_FUN(do_autogem);
DECLARE_DO_FUN(do_autohilt);
DECLARE_DO_FUN(do_autoall);
DECLARE_DO_FUN(do_newwho);
DECLARE_DO_FUN(do_xpup);
DECLARE_DO_FUN(do_samlearn);
DECLARE_DO_FUN(do_sword);
DECLARE_DO_FUN(do_tcolor);
DECLARE_DO_FUN(do_brief7);
DECLARE_DO_FUN(do_scroll);
DECLARE_DO_FUN(do_ghost);
DECLARE_DO_FUN(do_incognito);
DECLARE_DO_FUN(do_building);
DECLARE_DO_FUN(do_coding);

//Shop commands
DECLARE_DO_FUN(do_obuy);
DECLARE_DO_FUN(do_sell);
DECLARE_DO_FUN(do_olist);
DECLARE_DO_FUN(do_describe);

//Skyblade
DECLARE_DO_FUN(do_skyarmor);
DECLARE_DO_FUN(do_isauto);
DECLARE_DO_FUN(do_abilities);
DECLARE_DO_FUN(do_heavenlyrain);
DECLARE_DO_FUN(do_eaglestrength);
DECLARE_DO_FUN(do_hawkeyes);
DECLARE_DO_FUN(do_vulturevigor);
DECLARE_DO_FUN(do_skyforge);
DECLARE_DO_FUN(do_bladejump);
DECLARE_DO_FUN(do_pummel);
DECLARE_DO_FUN(do_cloudsblessing);
DECLARE_DO_FUN(do_dragonx);
DECLARE_DO_FUN(do_wyrmroar);
DECLARE_DO_FUN(do_divestrike);
DECLARE_DO_FUN(do_bladetalk);

//Ghoul
DECLARE_DO_FUN(do_realms);
DECLARE_DO_FUN(do_gcreate);
DECLARE_DO_FUN(do_gore);
DECLARE_DO_FUN(do_ravage);
DECLARE_DO_FUN(do_sdecay);
DECLARE_DO_FUN(do_sdeath);
DECLARE_DO_FUN(do_ghouleq);
DECLARE_DO_FUN(do_swampwalk);

//Draconians
DECLARE_DO_FUN(do_dragonorb);
DECLARE_DO_FUN(do_fieryeyes);
DECLARE_DO_FUN(do_spikes);
DECLARE_DO_FUN(do_deathstate);
DECLARE_DO_FUN(do_dclaws);
DECLARE_DO_FUN(do_headbash);
DECLARE_DO_FUN(do_acidblood);
DECLARE_DO_FUN(do_lizardform);
DECLARE_DO_FUN(do_venomspit);
DECLARE_DO_FUN(do_dscales);
DECLARE_DO_FUN(do_gasbreath);
DECLARE_DO_FUN(do_firebreath);
DECLARE_DO_FUN(do_dtail);
DECLARE_DO_FUN(do_dragonrun);

//Wizards
DECLARE_DO_FUN(do_chaos);
DECLARE_DO_FUN(do_arts);
DECLARE_DO_FUN(do_thunderclap);
DECLARE_DO_FUN(do_hellblades);
DECLARE_DO_FUN(do_wrath);
DECLARE_DO_FUN(do_firebolt);
DECLARE_DO_FUN(do_pshift);
DECLARE_DO_FUN(do_vortex);
DECLARE_DO_FUN(do_plost);
DECLARE_DO_FUN(do_icelance);
DECLARE_DO_FUN(do_negativity);

//Thieves
DECLARE_DO_FUN(do_thiefskills);
DECLARE_DO_FUN(do_rolldice);
DECLARE_DO_FUN(do_tconceal);
DECLARE_DO_FUN(do_gleam);
DECLARE_DO_FUN(do_bstab);
DECLARE_DO_FUN(do_tdagger);
DECLARE_DO_FUN(do_thiefeq);
DECLARE_DO_FUN(do_dtrick);
DECLARE_DO_FUN(do_poisonblade);
DECLARE_DO_FUN(do_shadowmove);

//Sorcerers
DECLARE_DO_FUN(do_mageskill);
DECLARE_DO_FUN(do_specialize);
DECLARE_DO_FUN(do_mindblank);
DECLARE_DO_FUN(do_target);
DECLARE_DO_FUN(do_magespells);
DECLARE_DO_FUN(do_sorchant);

// Shadows
DECLARE_DO_FUN(do_moonstrike);
DECLARE_DO_FUN(do_shadowthrust);
DECLARE_DO_FUN(do_dirtthrow);
DECLARE_DO_FUN(do_gutcutter);
DECLARE_DO_FUN(do_soulreaper);
DECLARE_DO_FUN(do_knifespin);
DECLARE_DO_FUN(do_wakasashislice);
DECLARE_DO_FUN(do_caltrops);
DECLARE_DO_FUN(do_soultarget);
DECLARE_DO_FUN(do_shadowlearn);
DECLARE_DO_FUN(do_soulseek);

// Priests
DECLARE_DO_FUN(do_pgain);
DECLARE_DO_FUN(do_priestform);
DECLARE_DO_FUN(do_ascend);
DECLARE_DO_FUN(do_godbless);
DECLARE_DO_FUN(do_godheal);
DECLARE_DO_FUN(do_godstrike);
DECLARE_DO_FUN(do_grheal);
DECLARE_DO_FUN(do_grstorm);
DECLARE_DO_FUN(do_healstorm);
DECLARE_DO_FUN(do_holyaura);
DECLARE_DO_FUN(do_majorheal);
DECLARE_DO_FUN(do_minheal);
DECLARE_DO_FUN(do_minstorm);
DECLARE_DO_FUN(do_pwho);
DECLARE_DO_FUN(do_turnundead);
DECLARE_DO_FUN(do_godpeace);

/* Jedi */
DECLARE_DO_FUN(do_jstate);
DECLARE_DO_FUN(do_jfight);
DECLARE_DO_FUN(do_fpush);
DECLARE_DO_FUN(do_channelforce);
DECLARE_DO_FUN(do_jstep);
DECLARE_DO_FUN(do_jediarmor);
DECLARE_DO_FUN(do_jedilearn);
DECLARE_DO_FUN(do_fshield);
DECLARE_DO_FUN(do_jediskill);
DECLARE_DO_FUN(do_jlevitate);
DECLARE_DO_FUN(do_jsight);
DECLARE_DO_FUN(do_jeyes);
DECLARE_DO_FUN(do_jsense);
DECLARE_DO_FUN(do_jfocus);
DECLARE_DO_FUN(do_jwalk);
DECLARE_DO_FUN(do_jhealing);
DECLARE_DO_FUN(do_jsummon);
DECLARE_DO_FUN(do_jsault);
DECLARE_DO_FUN(do_jreverse);
DECLARE_DO_FUN(do_jkneel);
DECLARE_DO_FUN(do_jstab);
DECLARE_DO_FUN(do_jhigh);
DECLARE_DO_FUN(do_jupper);
DECLARE_DO_FUN(do_jbutterfly);
DECLARE_DO_FUN(do_jcross);
DECLARE_DO_FUN(do_jslash);
DECLARE_DO_FUN(do_jcircle);
DECLARE_DO_FUN(do_jedishield);
DECLARE_DO_FUN(do_jedisleep);
DECLARE_DO_FUN(do_fspeed);
DECLARE_DO_FUN(do_jtouch);

DECLARE_DO_FUN(do_dismember);
DECLARE_DO_FUN(do_prename);
DECLARE_DO_FUN(do_delevel);
DECLARE_DO_FUN(do_psycho);
DECLARE_DO_FUN(do_viewtier);
DECLARE_DO_FUN(do_vote);
DECLARE_DO_FUN(do_changes);
DECLARE_DO_FUN(do_delchange);
DECLARE_DO_FUN(do_addchange);
DECLARE_DO_FUN(do_checkpk);
DECLARE_DO_FUN(do_oldlevel);
DECLARE_DO_FUN(do_spinebreak);
DECLARE_DO_FUN(do_beep);
DECLARE_DO_FUN(do_freevnums);
DECLARE_DO_FUN(do_unrestore);
DECLARE_DO_FUN(do_daynight);
DECLARE_DO_FUN(do_auction);
DECLARE_DO_FUN(do_wrlist);
DECLARE_DO_FUN(do_pathfind);
DECLARE_DO_FUN(do_mxp);
DECLARE_DO_FUN(do_pfile);
DECLARE_DO_FUN(jope_done);
DECLARE_DO_FUN(do_pinch);
DECLARE_DO_FUN(do_gsocial);
DECLARE_DO_FUN(jope_exp);
DECLARE_DO_FUN(do_ready);
DECLARE_DO_FUN(do_shop);
DECLARE_DO_FUN(do_autoslab);
DECLARE_DO_FUN(do_autosigil);
DECLARE_DO_FUN(do_project);
DECLARE_DO_FUN(do_connect);
DECLARE_DO_FUN(do_buy);
DECLARE_DO_FUN(do_manasuck);
DECLARE_DO_FUN(do_trivia);
DECLARE_DO_FUN(do_qtalk);
DECLARE_DO_FUN(do_list);
DECLARE_DO_FUN(jope_extra);
DECLARE_DO_FUN(jope_show);
DECLARE_DO_FUN(do_newscore);
DECLARE_DO_FUN(jope_list);
DECLARE_DO_FUN(do_map);
DECLARE_DO_FUN(do_autohead);
DECLARE_DO_FUN(do_bet);
DECLARE_DO_FUN(jope_spells);
DECLARE_DO_FUN(jope_stances);
DECLARE_DO_FUN(do_sing);
DECLARE_DO_FUN(jope_weapons);
DECLARE_DO_FUN(jope_action);
DECLARE_DO_FUN(jope_inventory);
DECLARE_DO_FUN(jope_drop);
DECLARE_DO_FUN(jope_fix);
DECLARE_DO_FUN(jope_jflags);
DECLARE_DO_FUN(jope_equipment);
DECLARE_DO_FUN(jope_get);
DECLARE_DO_FUN(jope_look);
DECLARE_DO_FUN(jope_remove);
DECLARE_DO_FUN(jope_wear);
DECLARE_DO_FUN(jope_newbits);
DECLARE_DO_FUN(jope_act);
DECLARE_DO_FUN(jope_qps);
DECLARE_DO_FUN(jope_primal);
DECLARE_DO_FUN(jope_level);
DECLARE_DO_FUN(jope_trust);
DECLARE_DO_FUN(jope_hit);
DECLARE_DO_FUN(jope_move);
DECLARE_DO_FUN(jope_mana);
DECLARE_DO_FUN(jope_pkill);
DECLARE_DO_FUN(jope_pdeath);
DECLARE_DO_FUN(jope_mkill);
DECLARE_DO_FUN(jope_mdeath);
DECLARE_DO_FUN(do_ccenter);
DECLARE_DO_FUN(do_autoset);
DECLARE_DO_FUN(do_clearflags);
DECLARE_DO_FUN(do_qcomplete);
DECLARE_DO_FUN(do_sound);
DECLARE_DO_FUN(do_qgain);
DECLARE_DO_FUN(do_addpoll);
DECLARE_DO_FUN(do_displayvotes);
DECLARE_DO_FUN(do_createbase);
DECLARE_DO_FUN(do_showquest);
DECLARE_DO_FUN(do_stanceset);
DECLARE_DO_FUN(do_spellset);

/* New Commands */
DECLARE_DO_FUN(do_resistance);
DECLARE_DO_FUN(do_affects);
DECLARE_DO_FUN(do_chands);
DECLARE_DO_FUN(do_resetpassword);
DECLARE_DO_FUN(do_pstat);
DECLARE_DO_FUN(do_implag);
DECLARE_DO_FUN(do_toggle);
DECLARE_DO_FUN(do_history);
DECLARE_DO_FUN(do_setclass);
DECLARE_DO_FUN(do_lastmessage);
DECLARE_DO_FUN(do_faith);
DECLARE_DO_FUN(do_showsilence);
DECLARE_DO_FUN(do_trusted);
DECLARE_DO_FUN(do_aggrocheck);
DECLARE_DO_FUN(do_showsounds);
DECLARE_DO_FUN(do_showcompress);
DECLARE_DO_FUN(do_openthearena);
DECLARE_DO_FUN(do_ragnarok);
DECLARE_DO_FUN(do_dblbuy);
DECLARE_DO_FUN(do_timer);
DECLARE_DO_FUN(do_selfdelete);
DECLARE_DO_FUN(do_vt100);
DECLARE_DO_FUN(do_artisteal);
DECLARE_DO_FUN(do_newban);
DECLARE_DO_FUN(do_newallow);
DECLARE_DO_FUN(do_logstat);
DECLARE_DO_FUN(do_logstatclear);
DECLARE_DO_FUN(do_ignore);
DECLARE_DO_FUN(do_policy);
DECLARE_DO_FUN(do_arenaset);
DECLARE_DO_FUN(do_challenge);
DECLARE_DO_FUN(do_decline);
DECLARE_DO_FUN(do_accept2);
DECLARE_DO_FUN(do_fortressstats);


/* SPIDER DROID */
DECLARE_DO_FUN(do_cubeform);
DECLARE_DO_FUN(do_cyborgeq);
DECLARE_DO_FUN(do_stuntubes);
DECLARE_DO_FUN(do_infravision);
DECLARE_DO_FUN(do_transmit);
DECLARE_DO_FUN(do_collective);

/* police */
DECLARE_DO_FUN(do_psilence);
DECLARE_DO_FUN(do_plog);

/* Mages */
DECLARE_DO_FUN(do_thirdeye);
DECLARE_DO_FUN(do_objectgate);
DECLARE_DO_FUN(do_discharge);
DECLARE_DO_FUN(do_enchant);


// Vampies
// Daimoinon
DECLARE_DO_FUN(do_guardian);
DECLARE_DO_FUN(do_fear);
DECLARE_DO_FUN(do_portal);
DECLARE_DO_FUN(do_curse);
DECLARE_DO_FUN(do_vtwist);

// Melpominee
DECLARE_DO_FUN(do_scream);

// Chimerstry
DECLARE_DO_FUN(do_mirror);
DECLARE_DO_FUN(do_formillusion);
DECLARE_DO_FUN(do_control);
//DECLARE_DO_FUN( do_objmask    );

// Necromancy
DECLARE_DO_FUN(do_thanatosis);
DECLARE_DO_FUN(do_preserve);
DECLARE_DO_FUN(do_spiritgate);
DECLARE_DO_FUN(do_spiritguard);
DECLARE_DO_FUN(do_zombie);

// Thanatosis
DECLARE_DO_FUN(do_hagswrinkles);
DECLARE_DO_FUN(do_rot);
DECLARE_DO_FUN(do_withering);
DECLARE_DO_FUN(do_drain);

// Auspex
DECLARE_DO_FUN(do_truesight);
DECLARE_DO_FUN(do_readaura);
DECLARE_DO_FUN(do_scry);
DECLARE_DO_FUN(do_astralwalk);
DECLARE_DO_FUN(do_unveil);

// Obfuscate
DECLARE_DO_FUN(do_vanish);
DECLARE_DO_FUN(do_mask);
DECLARE_DO_FUN(do_shield);
DECLARE_DO_FUN(do_mortal);
DECLARE_DO_FUN(do_objectmask);


/* Garou */

// Persuasion
DECLARE_DO_FUN(do_staredown);
DECLARE_DO_FUN(do_disquiet);
DECLARE_DO_FUN(do_reshape);
DECLARE_DO_FUN(do_cocoon);
DECLARE_DO_FUN(do_burrow);
DECLARE_DO_FUN(do_nightsight);
DECLARE_DO_FUN(do_wither);
DECLARE_DO_FUN(do_totemgift);

DECLARE_DO_FUN(do_razorclaws);


/* End of Garou */

DECLARE_DO_FUN(do_research);
DECLARE_DO_FUN(do_disciplines);
DECLARE_DO_FUN(do_top);
DECLARE_DO_FUN(do_topclear);
DECLARE_DO_FUN(do_classself);
DECLARE_DO_FUN(do_knee);
DECLARE_DO_FUN(do_elbow);
DECLARE_DO_FUN(do_sweep);
DECLARE_DO_FUN(do_reverse);
DECLARE_DO_FUN(do_palmstrike);
DECLARE_DO_FUN(do_shinkick);
DECLARE_DO_FUN(do_chi);
DECLARE_DO_FUN(do_thrustkick);
DECLARE_DO_FUN(do_spinkick);
DECLARE_DO_FUN(do_backfist);
DECLARE_DO_FUN(do_spiritpower);
DECLARE_DO_FUN(do_healingtouch);
DECLARE_DO_FUN(do_deathtouch);
DECLARE_DO_FUN(do_relax);
DECLARE_DO_FUN(do_relset);

DECLARE_DO_FUN(do_mitsukeru);
DECLARE_DO_FUN(do_koryou);
DECLARE_DO_FUN(do_udc);
DECLARE_DO_FUN(do_labour);
DECLARE_DO_FUN(do_abortion);
DECLARE_DO_FUN(do_god_fight);
DECLARE_DO_FUN(do_damn);
DECLARE_DO_FUN(do_darkness);
DECLARE_DO_FUN(do_refresh);
DECLARE_DO_FUN(do_vt102);
DECLARE_DO_FUN(do_retrn);
DECLARE_DO_FUN(do_far);
DECLARE_DO_FUN(do_dragon);
DECLARE_DO_FUN(do_forget);
DECLARE_DO_FUN(do_forge);
DECLARE_DO_FUN(do_spew);
DECLARE_DO_FUN(do_spiderform);
DECLARE_DO_FUN(do_flash);
DECLARE_DO_FUN(do_infirmity);
DECLARE_DO_FUN(do_coil);
DECLARE_DO_FUN(do_tide);
DECLARE_DO_FUN(do_death);
DECLARE_DO_FUN(do_acid);
DECLARE_DO_FUN(do_awe);
DECLARE_DO_FUN(do_satan);
/*ninjas*/
DECLARE_DO_FUN(do_ninjaarmor);
DECLARE_DO_FUN(do_hara_kiri);
DECLARE_DO_FUN(do_miktalk);
DECLARE_DO_FUN(do_principles);
DECLARE_DO_FUN(do_michi);
DECLARE_DO_FUN(do_kakusu);
DECLARE_DO_FUN(do_kanzuite);
DECLARE_DO_FUN(do_strangle);
DECLARE_DO_FUN(do_mienaku);
DECLARE_DO_FUN(do_flashbomb);
DECLARE_DO_FUN(do_belt);
DECLARE_DO_FUN(do_bomuzite);
DECLARE_DO_FUN(do_tsume);
DECLARE_DO_FUN(do_circle);
DECLARE_DO_FUN(do_omni);
DECLARE_DO_FUN(do_frostbreath);
DECLARE_DO_FUN(do_dgate);
DECLARE_DO_FUN(do_tick);
DECLARE_DO_FUN(do_resetarea);
DECLARE_DO_FUN(do_graft);
DECLARE_DO_FUN(do_blink);
DECLARE_DO_FUN(do_dinferno);
DECLARE_DO_FUN(do_immolate);
DECLARE_DO_FUN(do_seed);
DECLARE_DO_FUN(do_hellfire);
DECLARE_DO_FUN(do_newbiepack);
DECLARE_DO_FUN(do_snatch);
DECLARE_DO_FUN(do_runeeq);
DECLARE_DO_FUN(do_afk);
DECLARE_DO_FUN(do_unnerve);
DECLARE_DO_FUN(do_wfreeze);
DECLARE_DO_FUN(do_chaosport);
DECLARE_DO_FUN(do_caust);
DECLARE_DO_FUN(do_gust);
DECLARE_DO_FUN(do_entomb);
DECLARE_DO_FUN(do_leech);
DECLARE_DO_FUN(do_configure);
DECLARE_DO_FUN(do_stealsoul);
DECLARE_DO_FUN(do_deathsense);
DECLARE_DO_FUN(do_glamour);
DECLARE_DO_FUN(do_prefix);

/* OLC Stuff Ooo */
DECLARE_DO_FUN(do_hlist);
DECLARE_DO_FUN(do_hset);
DECLARE_DO_FUN(do_ocreate);
DECLARE_DO_FUN(do_mcreate);
DECLARE_DO_FUN(do_redit);
DECLARE_DO_FUN(do_medit);
DECLARE_DO_FUN(do_mpedit);
DECLARE_DO_FUN(do_aedit);
DECLARE_DO_FUN(do_oedit);
DECLARE_DO_FUN(do_sedit);
DECLARE_DO_FUN(do_asave);
DECLARE_DO_FUN(do_alist);
DECLARE_DO_FUN(do_resets);
DECLARE_DO_FUN(do_mpdump);
DECLARE_DO_FUN(do_rpdump);
DECLARE_DO_FUN(do_opdump);

DECLARE_DO_FUN(do_bloodwall);
DECLARE_DO_FUN(do_bloodrod);

DECLARE_DO_FUN(do_sclaws);
/* Protean */
DECLARE_DO_FUN(do_healing);

/* Obtene */
DECLARE_DO_FUN(do_grab);
DECLARE_DO_FUN(do_shadowgaze);

/* Luna Powers */

DECLARE_DO_FUN(do_klaive);
DECLARE_DO_FUN(do_gklaive);
DECLARE_DO_FUN(do_tribe);
DECLARE_DO_FUN(do_flameclaws);
DECLARE_DO_FUN(do_moonarmour);
DECLARE_DO_FUN(do_motherstouch);
DECLARE_DO_FUN(do_gmotherstouch);
DECLARE_DO_FUN(do_moongate);
DECLARE_DO_FUN(do_moonbeam);

/* No more Luna Powers */

DECLARE_DO_FUN(do_skin);
DECLARE_DO_FUN(do_bonemod);
DECLARE_DO_FUN(do_cauldron);
DECLARE_DO_FUN(do_flamehands);
DECLARE_DO_FUN(do_conceal);
DECLARE_DO_FUN(do_shroud);
DECLARE_DO_FUN(do_share);
DECLARE_DO_FUN(do_frenzy);
DECLARE_DO_FUN(do_chaosblast);
DECLARE_DO_FUN(do_accept);
DECLARE_DO_FUN(do_obtain);
DECLARE_DO_FUN(do_warps);

/* End of Dunkirk's Shit */

DECLARE_DO_FUN(do_activate);
DECLARE_DO_FUN(do_alignment);

DECLARE_DO_FUN(do_alias);
DECLARE_DO_FUN(do_showalias);
DECLARE_DO_FUN(do_removealias);
DECLARE_DO_FUN(do_trigger);
DECLARE_DO_FUN(do_allow);
DECLARE_DO_FUN(do_bounty);
DECLARE_DO_FUN(do_bountylist);
DECLARE_DO_FUN(do_ansi);
DECLARE_DO_FUN(do_areas);
DECLARE_DO_FUN(do_artifact);
DECLARE_DO_FUN(do_at);
DECLARE_DO_FUN(do_autoexit);
DECLARE_DO_FUN(do_autoloot);
DECLARE_DO_FUN(do_autosac);
DECLARE_DO_FUN(do_autosave);
DECLARE_DO_FUN(do_autostance);
DECLARE_DO_FUN(do_backstab);
DECLARE_DO_FUN(do_bamfin);
DECLARE_DO_FUN(do_bamfout);
DECLARE_DO_FUN(do_ban);
DECLARE_DO_FUN(do_beastlike);
DECLARE_DO_FUN(do_berserk);
DECLARE_DO_FUN(do_berserk2);
DECLARE_DO_FUN(do_bind);
DECLARE_DO_FUN(do_birth);
DECLARE_DO_FUN(do_autobones);
DECLARE_DO_FUN(do_blindfold);
DECLARE_DO_FUN(do_bloodline);
DECLARE_DO_FUN(do_brandish);
DECLARE_DO_FUN(do_breakup);
DECLARE_DO_FUN(do_brief);
DECLARE_DO_FUN(do_brief2);
DECLARE_DO_FUN(do_brief3);
DECLARE_DO_FUN(do_brief4);
DECLARE_DO_FUN(do_brief5);
DECLARE_DO_FUN(do_brief6);
DECLARE_DO_FUN(do_bug);
DECLARE_DO_FUN(do_call);
DECLARE_DO_FUN(do_calm);
DECLARE_DO_FUN(do_cast);
DECLARE_DO_FUN(do_change);
DECLARE_DO_FUN(do_changelight);
DECLARE_DO_FUN(do_channels);
DECLARE_DO_FUN(do_compress);
DECLARE_DO_FUN(do_compres);
DECLARE_DO_FUN(do_knightarmor);
DECLARE_DO_FUN(do_gain);
DECLARE_DO_FUN(do_weaponpractice);
DECLARE_DO_FUN(do_aura);
DECLARE_DO_FUN(do_powerword);
DECLARE_DO_FUN(do_knighttalk);
DECLARE_DO_FUN(do_ride);
DECLARE_DO_FUN(do_unholyrite);
/* Mages */
DECLARE_DO_FUN(do_chant);
DECLARE_DO_FUN(do_reveal);
DECLARE_DO_FUN(do_magics);
DECLARE_DO_FUN(do_blast);
DECLARE_DO_FUN(do_teleport);
DECLARE_DO_FUN(do_invoke);
DECLARE_DO_FUN(do_magearmor);
DECLARE_DO_FUN(do_chaosmagic);
DECLARE_DO_FUN(do_mirage);

/* Angels */
DECLARE_DO_FUN(do_gpeace);
DECLARE_DO_FUN(do_innerpeace);
DECLARE_DO_FUN(do_houseofgod);
DECLARE_DO_FUN(do_angelicaura);
DECLARE_DO_FUN(do_gbanish);
DECLARE_DO_FUN(do_harmony);
DECLARE_DO_FUN(do_gsenses);
DECLARE_DO_FUN(do_gfavor);
DECLARE_DO_FUN(do_forgivness);
DECLARE_DO_FUN(do_martyr);
DECLARE_DO_FUN(do_swoop);
DECLARE_DO_FUN(do_awings);
DECLARE_DO_FUN(do_halo);
DECLARE_DO_FUN(do_sinsofthepast);
DECLARE_DO_FUN(do_eyeforaneye);
DECLARE_DO_FUN(do_angelicarmor);
DECLARE_DO_FUN(do_angeltalk);
DECLARE_DO_FUN(do_touchofgod);
DECLARE_DO_FUN(do_spiritform);

/* Shapeshifters */
DECLARE_DO_FUN(do_shift);
DECLARE_DO_FUN(do_formlearn);
DECLARE_DO_FUN(do_camouflage);
DECLARE_DO_FUN(do_shapeshift);
DECLARE_DO_FUN(do_hatform);
DECLARE_DO_FUN(do_mistwalk);
DECLARE_DO_FUN(do_shapearmor);
DECLARE_DO_FUN(do_shaperoar);
DECLARE_DO_FUN(do_charge);
DECLARE_DO_FUN(do_faerieblink);
DECLARE_DO_FUN(do_stomp);
DECLARE_DO_FUN(do_faeriecurse);
DECLARE_DO_FUN(do_phase);
DECLARE_DO_FUN(do_breath);

DECLARE_DO_FUN(do_monkarmor);
DECLARE_DO_FUN(do_vampirearmor);
DECLARE_DO_FUN(do_purification);
DECLARE_DO_FUN(do_chat);
DECLARE_DO_FUN(do_claim);
DECLARE_DO_FUN(do_flame);

DECLARE_DO_FUN(do_arenastats);
DECLARE_DO_FUN(do_arenajoin);
DECLARE_DO_FUN(do_resign);

DECLARE_DO_FUN(do_kingdoms);
DECLARE_DO_FUN(do_ktalk);
DECLARE_DO_FUN(do_kinduct);
DECLARE_DO_FUN(do_wantkingdom);
DECLARE_DO_FUN(do_kset);
DECLARE_DO_FUN(do_koutcast);
DECLARE_DO_FUN(do_kstats);
DECLARE_DO_FUN(do_kingset);
DECLARE_DO_FUN(do_decwar);
DECLARE_DO_FUN(do_warlist);
DECLARE_DO_FUN(do_warpeace);
DECLARE_DO_FUN(do_krecall);
DECLARE_DO_FUN(do_kpromote);
DECLARE_DO_FUN(do_kwithdraw);
DECLARE_DO_FUN(do_defect);
DECLARE_DO_FUN(do_leader);
DECLARE_DO_FUN(do_leaderclear);
DECLARE_DO_FUN(do_clandisc);
DECLARE_DO_FUN(do_tourney);
DECLARE_DO_FUN(do_introduce);
DECLARE_DO_FUN(do_claws);
DECLARE_DO_FUN(do_class);
DECLARE_DO_FUN(do_clearstats);
DECLARE_DO_FUN(do_clearstats2);
DECLARE_DO_FUN(do_close);
DECLARE_DO_FUN(do_command);
DECLARE_DO_FUN(do_commands);
DECLARE_DO_FUN(do_config);
DECLARE_DO_FUN(do_consent);
DECLARE_DO_FUN(do_consider);
DECLARE_DO_FUN(do_cprompt);
DECLARE_DO_FUN(do_crack);
DECLARE_DO_FUN(do_create);
DECLARE_DO_FUN(do_credits);
DECLARE_DO_FUN(do_darkheart);
DECLARE_DO_FUN(do_vampdarkness);
DECLARE_DO_FUN(do_decapitate);
DECLARE_DO_FUN(do_tiercap);
DECLARE_DO_FUN(do_gifts);
DECLARE_DO_FUN(do_trueform);
DECLARE_DO_FUN(do_cone);
DECLARE_DO_FUN(do_dstake);
DECLARE_DO_FUN(do_demonarmour);
DECLARE_DO_FUN(do_deny);
DECLARE_DO_FUN(do_description);
DECLARE_DO_FUN(do_diagnose);
DECLARE_DO_FUN(do_dismount);
DECLARE_DO_FUN(do_disable);
DECLARE_DO_FUN(do_disarm);
DECLARE_DO_FUN(do_disconnect);
DECLARE_DO_FUN(do_divorce);
DECLARE_DO_FUN(do_down);
DECLARE_DO_FUN(do_draw);
DECLARE_DO_FUN(do_drink);
DECLARE_DO_FUN(do_drop);

DECLARE_DO_FUN(do_settie);
DECLARE_DO_FUN(do_setlogout);
DECLARE_DO_FUN(do_setlogin);
DECLARE_DO_FUN(do_setdecap);
DECLARE_DO_FUN(do_setavatar);
DECLARE_DO_FUN(do_worship);
DECLARE_DO_FUN(do_forsake);
DECLARE_DO_FUN(do_setrank);
DECLARE_DO_FUN(do_relbuy);
DECLARE_DO_FUN(do_gzone);
DECLARE_DO_FUN(do_darmor);
DECLARE_DO_FUN(do_farmor);
DECLARE_DO_FUN(do_holyword);
DECLARE_DO_FUN(do_areaaffects);
DECLARE_DO_FUN(do_thorns);
DECLARE_DO_FUN(do_rpurge);
DECLARE_DO_FUN(do_deja);
DECLARE_DO_FUN(do_holysymbol);
DECLARE_DO_FUN(do_areaheal);
DECLARE_DO_FUN(do_badmoon);
DECLARE_DO_FUN(do_landscape);
DECLARE_DO_FUN(do_relpowers);
DECLARE_DO_FUN(do_hdesire);
DECLARE_DO_FUN(do_dscry);
DECLARE_DO_FUN(do_pestilence);
DECLARE_DO_FUN(do_banishment);
DECLARE_DO_FUN(do_starsight);
DECLARE_DO_FUN(do_gaia);
DECLARE_DO_FUN(do_dbattle);
DECLARE_DO_FUN(do_callwild);
DECLARE_DO_FUN(do_madness);
DECLARE_DO_FUN(do_gfury);
DECLARE_DO_FUN(do_subvert);
DECLARE_DO_FUN(do_rebirth);
DECLARE_DO_FUN(do_supernova);

// drows
DECLARE_DO_FUN(send_grantlist);
DECLARE_DO_FUN(do_drowcreate);
DECLARE_DO_FUN(do_heal);
DECLARE_DO_FUN(do_drowfire);
DECLARE_DO_FUN(do_drowhate);
DECLARE_DO_FUN(do_drowpowers);
DECLARE_DO_FUN(do_drowshield);
DECLARE_DO_FUN(do_lloth);
DECLARE_DO_FUN(do_drowsight);
DECLARE_DO_FUN(do_darktendrils);
DECLARE_DO_FUN(do_fightdance);
DECLARE_DO_FUN(do_ntrust);
DECLARE_DO_FUN(do_grant);
DECLARE_DO_FUN(do_earthmeld);
DECLARE_DO_FUN(do_east);
DECLARE_DO_FUN(do_eat);
DECLARE_DO_FUN(do_echo);
DECLARE_DO_FUN(do_empty);
DECLARE_DO_FUN(do_escape);
DECLARE_DO_FUN(do_emote);
DECLARE_DO_FUN(do_enter);
DECLARE_DO_FUN(do_equipment);
DECLARE_DO_FUN(do_evileye);
DECLARE_DO_FUN(do_examine);
DECLARE_DO_FUN(do_exits);
DECLARE_DO_FUN(do_exlist);
DECLARE_DO_FUN(do_eyespy);
DECLARE_DO_FUN(do_familiar);
DECLARE_DO_FUN(do_fangs);
DECLARE_DO_FUN(do_favor);
DECLARE_DO_FUN(do_favour);
DECLARE_DO_FUN(do_fcommand);
DECLARE_DO_FUN(do_fightstyle);
DECLARE_DO_FUN(do_fileupdate);
DECLARE_DO_FUN(do_fill);
DECLARE_DO_FUN(do_finger);
DECLARE_DO_FUN(do_flee);
DECLARE_DO_FUN(do_flex);
DECLARE_DO_FUN(do_follow);
DECLARE_DO_FUN(do_force);
DECLARE_DO_FUN(do_asperson);
DECLARE_DO_FUN(do_offline);
DECLARE_DO_FUN(do_forceauto);
DECLARE_DO_FUN(do_freeze);
DECLARE_DO_FUN(do_earthshatter);
DECLARE_DO_FUN(do_confuse);
DECLARE_DO_FUN(do_gag);
DECLARE_DO_FUN(do_garotte);
DECLARE_DO_FUN(do_dark_garotte);
DECLARE_DO_FUN(do_get);
DECLARE_DO_FUN(do_generation);
DECLARE_DO_FUN(do_gift);
DECLARE_DO_FUN(do_give);
DECLARE_DO_FUN(do_goto);
DECLARE_DO_FUN(do_group);
DECLARE_DO_FUN(do_gtell);   /*
                             * DECLARE_DO_FUN(do_heal ); */
DECLARE_DO_FUN(do_help);
DECLARE_DO_FUN(do_hide);
DECLARE_DO_FUN(do_hightalk);
DECLARE_DO_FUN(do_monktalk);
DECLARE_DO_FUN(do_holylight);
DECLARE_DO_FUN(do_home);
DECLARE_DO_FUN(do_horns);
DECLARE_DO_FUN(do_howl);
DECLARE_DO_FUN(do_telepath);
DECLARE_DO_FUN(do_communicate);
DECLARE_DO_FUN(do_huh);
DECLARE_DO_FUN(do_humanform);
DECLARE_DO_FUN(do_humanity);
DECLARE_DO_FUN(do_hunt);
DECLARE_DO_FUN(do_hurl);
DECLARE_DO_FUN(do_idea);
DECLARE_DO_FUN(do_immune);
DECLARE_DO_FUN(do_immtalk);
DECLARE_DO_FUN(do_inconnu);
DECLARE_DO_FUN(do_info);
DECLARE_DO_FUN(do_inpart);
DECLARE_DO_FUN(do_inventory);
DECLARE_DO_FUN(do_invis);
DECLARE_DO_FUN(do_kick);
DECLARE_DO_FUN(do_godsfavor);

/* Tanar'ri */
DECLARE_DO_FUN(do_earthquake);
DECLARE_DO_FUN(do_tornado);
DECLARE_DO_FUN(do_infernal);
DECLARE_DO_FUN(do_bloodsac);
DECLARE_DO_FUN(do_enmity);
DECLARE_DO_FUN(do_enrage);
DECLARE_DO_FUN(do_booming);
DECLARE_DO_FUN(do_chaosgate);
DECLARE_DO_FUN(do_fury);
DECLARE_DO_FUN(do_tantalk);
DECLARE_DO_FUN(do_taneq);
DECLARE_DO_FUN(do_lavablast);
DECLARE_DO_FUN(do_chaossurge);

/* Fae */
DECLARE_DO_FUN(do_reform);
DECLARE_DO_FUN(do_faetalk);
DECLARE_DO_FUN(do_elementalform);
DECLARE_DO_FUN(do_ancients);
DECLARE_DO_FUN(do_conjurebow);
DECLARE_DO_FUN(do_will);
DECLARE_DO_FUN(do_plasma2);
DECLARE_DO_FUN(do_matter);
DECLARE_DO_FUN(do_energy);
DECLARE_DO_FUN(do_infuse);
DECLARE_DO_FUN(do_channel);
DECLARE_DO_FUN(do_djinn);
DECLARE_DO_FUN(do_genie);

/* Hobbits */
DECLARE_DO_FUN(do_hobtalk);
DECLARE_DO_FUN(do_hobbitarmor);
DECLARE_DO_FUN(do_bettercook);
DECLARE_DO_FUN(do_dishes);
DECLARE_DO_FUN(do_cook);

/* Elementals */
DECLARE_DO_FUN(do_enflame);
DECLARE_DO_FUN(do_magma);
DECLARE_DO_FUN(do_naturesfury);
DECLARE_DO_FUN(do_disperce);
DECLARE_DO_FUN(do_enfold);
DECLARE_DO_FUN(do_hydro);
DECLARE_DO_FUN(do_evaporate);
DECLARE_DO_FUN(do_solidify);
DECLARE_DO_FUN(do_elemshift);
// DECLARE_DO_FUN(

/* Paladins */
DECLARE_DO_FUN(do_enlighten);
DECLARE_DO_FUN(do_pholyaura);
DECLARE_DO_FUN(do_retribution);
DECLARE_DO_FUN(do_pmight);
DECLARE_DO_FUN(do_pwrath);
DECLARE_DO_FUN(do_pblade);
DECLARE_DO_FUN(do_barmor);
DECLARE_DO_FUN(do_paladinpractice);
DECLARE_DO_FUN(do_holyvision);
DECLARE_DO_FUN(do_righteous);
DECLARE_DO_FUN(do_pbash);
DECLARE_DO_FUN(do_paladinleap);

/* Blade Master */
DECLARE_DO_FUN(do_bladegain);
DECLARE_DO_FUN(do_retort);
DECLARE_DO_FUN(do_portal);
DECLARE_DO_FUN(do_rapidstrike);
DECLARE_DO_FUN(do_pstrike);
DECLARE_DO_FUN(do_combatstyle);
// DECLARE_DO_FUN(
// DECLARE_DO_FUN(
// DECLARE_DO_FUN(

/* Dragons Stuff, GryphonHawk March 2003*/

DECLARE_DO_FUN(do_droar        );
DECLARE_DO_FUN(do_dgain        );
DECLARE_DO_FUN(do_fly          );
DECLARE_DO_FUN(do_dgem         );
DECLARE_DO_FUN(do_drackform    );
DECLARE_DO_FUN(do_hatch        );
DECLARE_DO_FUN(do_lick         );
DECLARE_DO_FUN(do_dconfuse     );
DECLARE_DO_FUN(do_peer         );
DECLARE_DO_FUN(do_consume      );
DECLARE_DO_FUN(do_age          );
DECLARE_DO_FUN(do_gbreath      );
DECLARE_DO_FUN(do_fbreath      );
DECLARE_DO_FUN(do_lbreath      );
DECLARE_DO_FUN(do_cbreath      );
DECLARE_DO_FUN(do_abreath      );
DECLARE_DO_FUN(do_nest         );
DECLARE_DO_FUN(do_dswoop       );
DECLARE_DO_FUN(do_deat         );
DECLARE_DO_FUN(do_dsight       );
DECLARE_DO_FUN(do_breathforge  );
/*  END DRAGONS */

/* Shinobi */
DECLARE_DO_FUN( do_jutsus);
DECLARE_DO_FUN( do_handmotions);
// DECLARE_DO_FUN( do_shinflag);
DECLARE_DO_FUN( do_shinobirank);
// DECLARE_DO_FUN( do_tag);

/* Zombie */
DECLARE_DO_FUN( do_evolve);
DECLARE_DO_FUN( do_satiate);
DECLARE_DO_FUN( do_shatter);
DECLARE_DO_FUN( do_explosion);
DECLARE_DO_FUN( do_zomscry);
DECLARE_DO_FUN( do_forsake);
DECLARE_DO_FUN( do_relinquish);
DECLARE_DO_FUN( do_spawn);
DECLARE_DO_FUN( do_deathvision);
DECLARE_DO_FUN( do_gravespawn);
DECLARE_DO_FUN( do_zombiesight);
DECLARE_DO_FUN( do_fleshportal);
DECLARE_DO_FUN( do_regrow);
DECLARE_DO_FUN( do_infest);
DECLARE_DO_FUN( do_assimilate);
DECLARE_DO_FUN( do_store);
DECLARE_DO_FUN( do_shed);
DECLARE_DO_FUN( do_haunt);
DECLARE_DO_FUN( do_noxious);
DECLARE_DO_FUN( do_fleshbond);
DECLARE_DO_FUN( do_zomtalk);

/* Giants */
DECLARE_DO_FUN(do_ggrow);
DECLARE_DO_FUN(do_bash);
DECLARE_DO_FUN(do_smack);
DECLARE_DO_FUN(do_thwack);
DECLARE_DO_FUN(do_giantarmor);
DECLARE_DO_FUN(do_dawnstrength);
DECLARE_DO_FUN(do_gsweep);
DECLARE_DO_FUN(do_giantgift);
DECLARE_DO_FUN(do_standfirm);
DECLARE_DO_FUN(do_earthpunch);
DECLARE_DO_FUN(do_revival);
DECLARE_DO_FUN(do_stoneshape);
DECLARE_DO_FUN(do_rumble);
DECLARE_DO_FUN(do_deathfrenzy);

/* Power Lich */
DECLARE_DO_FUN(do_callgolems);
DECLARE_DO_FUN(do_lore);
DECLARE_DO_FUN(do_studylore);
DECLARE_DO_FUN(do_lichtalk);
DECLARE_DO_FUN(do_chaosshield);
DECLARE_DO_FUN(do_summongolem);
DECLARE_DO_FUN(do_planartravel);
DECLARE_DO_FUN(do_planarstorm);
DECLARE_DO_FUN(do_powertransfer);
DECLARE_DO_FUN(do_polarity);
DECLARE_DO_FUN(do_chillhand);
DECLARE_DO_FUN(do_creepingdoom);
DECLARE_DO_FUN(do_painwreck);
DECLARE_DO_FUN(do_earthswallow);
DECLARE_DO_FUN(do_licharmor);
DECLARE_DO_FUN(do_soulsuck);
DECLARE_DO_FUN(do_pentagram);
DECLARE_DO_FUN(do_planeshift);

/* Monk */
DECLARE_DO_FUN(do_reseteq);
DECLARE_DO_FUN(do_mantra);
DECLARE_DO_FUN(do_wrathofgod);
DECLARE_DO_FUN(do_guide);
DECLARE_DO_FUN(do_makezerg);
DECLARE_DO_FUN(do_monk);
DECLARE_DO_FUN(do_zergpower);
DECLARE_DO_FUN(do_zclan);
DECLARE_DO_FUN(do_superinvis);
DECLARE_DO_FUN(do_supershield);
DECLARE_DO_FUN(do_celestial);
DECLARE_DO_FUN(do_godseye);
DECLARE_DO_FUN(do_godsbless);
DECLARE_DO_FUN(do_flaminghands);
DECLARE_DO_FUN(do_cloak);
DECLARE_DO_FUN(do_prayofages);
DECLARE_DO_FUN(do_darkblaze);
DECLARE_DO_FUN(do_adamantium);
DECLARE_DO_FUN(do_steelskin);
DECLARE_DO_FUN(do_godsheal);
DECLARE_DO_FUN(do_ghold);
DECLARE_DO_FUN(do_callgod);

DECLARE_DO_FUN(do_holytrain);
DECLARE_DO_FUN(do_holyvision);
DECLARE_DO_FUN(do_prayer);
DECLARE_DO_FUN(do_palmthrust);
DECLARE_DO_FUN(do_sacredinvis);
DECLARE_DO_FUN(do_kill);
DECLARE_DO_FUN(do_combatswitch);
DECLARE_DO_FUN(do_killperson);
DECLARE_DO_FUN(do_leap);
DECLARE_DO_FUN(do_lifespan);
DECLARE_DO_FUN(do_locate);
DECLARE_DO_FUN(do_lock);
DECLARE_DO_FUN(do_log);
DECLARE_DO_FUN(do_look);
DECLARE_DO_FUN(do_majesty);
DECLARE_DO_FUN(do_makepreg);
DECLARE_DO_FUN(do_marry);
DECLARE_DO_FUN(do_meditate);
DECLARE_DO_FUN(do_memory);
DECLARE_DO_FUN(do_mfind);
DECLARE_DO_FUN(do_mload);
DECLARE_DO_FUN(do_mount);
DECLARE_DO_FUN(do_mortalvamp);
DECLARE_DO_FUN(do_mset);
DECLARE_DO_FUN(do_undeny);
DECLARE_DO_FUN(do_mstat);
DECLARE_DO_FUN(do_mpstat);
DECLARE_DO_FUN(do_opstat);
DECLARE_DO_FUN(do_rpstat);
DECLARE_DO_FUN(do_mwhere);
DECLARE_DO_FUN(do_music);
DECLARE_DO_FUN(do_north);
DECLARE_DO_FUN(do_oclone);
DECLARE_DO_FUN(do_ofind);
DECLARE_DO_FUN(do_oload);
DECLARE_DO_FUN(do_open);
DECLARE_DO_FUN(do_order);
DECLARE_DO_FUN(do_oreturn);
DECLARE_DO_FUN(do_outcast);
DECLARE_DO_FUN(do_owhere);
DECLARE_DO_FUN(do_reimb);
DECLARE_DO_FUN(do_reimburse);
DECLARE_DO_FUN(do_oset);
DECLARE_DO_FUN(do_ostat);
DECLARE_DO_FUN(do_oswitch);
DECLARE_DO_FUN(do_otransfer);
DECLARE_DO_FUN(do_champions);
DECLARE_DO_FUN(do_paradox);
DECLARE_DO_FUN(do_bully);
DECLARE_DO_FUN(do_password);
DECLARE_DO_FUN(do_peace);
DECLARE_DO_FUN(do_pick);
DECLARE_DO_FUN(do_plasma);
DECLARE_DO_FUN(do_pload);
DECLARE_DO_FUN(do_poison);
DECLARE_DO_FUN(do_possession);
DECLARE_DO_FUN(do_practice);
DECLARE_DO_FUN(do_pray);
DECLARE_DO_FUN(do_press);
DECLARE_DO_FUN(do_preturn);
DECLARE_DO_FUN(do_prompt);
DECLARE_DO_FUN(do_propose);
DECLARE_DO_FUN(do_pset);
DECLARE_DO_FUN(do_pull);
DECLARE_DO_FUN(do_punch);
DECLARE_DO_FUN(do_purge);
DECLARE_DO_FUN(do_put);
DECLARE_DO_FUN(do_quaff);
DECLARE_DO_FUN(do_qset);
DECLARE_DO_FUN(do_qstat);
DECLARE_DO_FUN(do_qtrust);
DECLARE_DO_FUN(do_quest);
DECLARE_DO_FUN(do_qui);
DECLARE_DO_FUN(do_quit);
DECLARE_DO_FUN(do_rage);
DECLARE_DO_FUN(do_copyover);
DECLARE_DO_FUN(do_recall);
DECLARE_DO_FUN(do_crecall);
DECLARE_DO_FUN(do_recho);
DECLARE_DO_FUN(do_recite);
DECLARE_DO_FUN(do_regenerate);
DECLARE_DO_FUN(do_release);
DECLARE_DO_FUN(do_relevel);
DECLARE_DO_FUN(do_nike);
DECLARE_DO_FUN(do_hreload);
DECLARE_DO_FUN(do_remove);
DECLARE_DO_FUN(do_rent);
DECLARE_DO_FUN(do_reply);
DECLARE_DO_FUN(do_report);
DECLARE_DO_FUN(do_rescue);
DECLARE_DO_FUN(do_rest);
DECLARE_DO_FUN(do_restore);
DECLARE_DO_FUN(do_return);
DECLARE_DO_FUN(do_roll);
DECLARE_DO_FUN(do_rset);
DECLARE_DO_FUN(do_rstat);
DECLARE_DO_FUN(do_sacrifice);
DECLARE_DO_FUN(do_safe);
DECLARE_DO_FUN(do_save);
DECLARE_DO_FUN(do_say);
DECLARE_DO_FUN(do_scan);
DECLARE_DO_FUN(do_stat);
DECLARE_DO_FUN(do_spit);
DECLARE_DO_FUN(do_tongue);
DECLARE_DO_FUN(do_mindblast);
DECLARE_DO_FUN(do_powers);
DECLARE_DO_FUN(do_score);
DECLARE_DO_FUN(do_weaplist);
DECLARE_DO_FUN(do_level);
DECLARE_DO_FUN(do_mastery);
DECLARE_DO_FUN(do_exp);
DECLARE_DO_FUN(do_upgrade);
DECLARE_DO_FUN(do_healme);
DECLARE_DO_FUN(do_pkpowers);
DECLARE_DO_FUN(do_gensteal);
DECLARE_DO_FUN(do_setstance);
DECLARE_DO_FUN(do_itemwearfind);
DECLARE_DO_FUN(do_specfind);
DECLARE_DO_FUN(do_multicheck);

DECLARE_DO_FUN(do_mudstat);
DECLARE_DO_FUN(do_pkscry);
DECLARE_DO_FUN(do_pkmind);
DECLARE_DO_FUN(do_pkcall);
DECLARE_DO_FUN(do_pkheal);
DECLARE_DO_FUN(do_pkaura);
DECLARE_DO_FUN(do_pkportal);
DECLARE_DO_FUN(do_pkhunter);
DECLARE_DO_FUN(do_pkobjscry);
DECLARE_DO_FUN(do_pkvision);

/* SPIDER DROIDS */
DECLARE_DO_FUN(do_chitter);
DECLARE_DO_FUN(do_spiderarmor);
DECLARE_DO_FUN(do_radarscan);
DECLARE_DO_FUN(do_battlestation);
DECLARE_DO_FUN(do_reload);
DECLARE_DO_FUN(do_implant);
DECLARE_DO_FUN(do_droidspeed);
DECLARE_DO_FUN(do_submachine);
DECLARE_DO_FUN(do_rocketblast);
DECLARE_DO_FUN(do_steam);

DECLARE_DO_FUN(do_serenity);
DECLARE_DO_FUN(do_serpent);
DECLARE_DO_FUN(do_shadowplane);
DECLARE_DO_FUN(do_shadowsight);
DECLARE_DO_FUN(do_shadowstep);
DECLARE_DO_FUN(do_shadowwalk);
DECLARE_DO_FUN(do_sheath);
DECLARE_DO_FUN(do_shutdow);
DECLARE_DO_FUN(do_shutdown);
DECLARE_DO_FUN(do_side);
DECLARE_DO_FUN(do_learn);
DECLARE_DO_FUN(do_stalk);
DECLARE_DO_FUN(do_bladespin);
DECLARE_DO_FUN(do_hologramtransfer);
DECLARE_DO_FUN(do_sign);


//Samurai
DECLARE_DO_FUN(do_martial);
DECLARE_DO_FUN(do_katana);
DECLARE_DO_FUN(do_focus);
DECLARE_DO_FUN(do_slide);
DECLARE_DO_FUN(do_sidestep);
DECLARE_DO_FUN(do_block);
DECLARE_DO_FUN(do_countermove);

DECLARE_DO_FUN(do_abstract);
DECLARE_DO_FUN(do_dronearmor);
DECLARE_DO_FUN(do_drool);
DECLARE_DO_FUN(do_mentalblock);
DECLARE_DO_FUN(do_mindwarp);
DECLARE_DO_FUN(do_realityshift);
DECLARE_DO_FUN(do_psionicburst);
DECLARE_DO_FUN(do_mindseye);
DECLARE_DO_FUN(do_samseye);
DECLARE_DO_FUN(do_horrid);
DECLARE_DO_FUN(do_distort);
DECLARE_DO_FUN(do_nightmare);
DECLARE_DO_FUN(do_memorytwist);
DECLARE_DO_FUN(do_visage);

DECLARE_DO_FUN(do_reltalk);
DECLARE_DO_FUN(do_dktalk);
DECLARE_DO_FUN(do_silence);
DECLARE_DO_FUN(do_sit);
DECLARE_DO_FUN(do_skill);
DECLARE_DO_FUN(do_sla);
DECLARE_DO_FUN(do_slay);
DECLARE_DO_FUN(do_slay2);
DECLARE_DO_FUN(do_sleep);
DECLARE_DO_FUN(do_slookup);
DECLARE_DO_FUN(do_spell);
DECLARE_DO_FUN(do_tendrils);
DECLARE_DO_FUN(do_lamprey);
DECLARE_DO_FUN(do_stake);
DECLARE_DO_FUN(do_stance);
DECLARE_DO_FUN(do_smother);
DECLARE_DO_FUN(do_sneak);
DECLARE_DO_FUN(do_snoop);
DECLARE_DO_FUN(do_socials);
DECLARE_DO_FUN(do_south);
DECLARE_DO_FUN(do_spy);
DECLARE_DO_FUN(do_spydirection);
DECLARE_DO_FUN(do_sset);
DECLARE_DO_FUN(do_stand);
DECLARE_DO_FUN(do_steal);
DECLARE_DO_FUN(do_summon);
DECLARE_DO_FUN(do_nosummon);
DECLARE_DO_FUN(do_notravel);
DECLARE_DO_FUN(do_switch);
DECLARE_DO_FUN(do_embrace);
DECLARE_DO_FUN(do_diablerise);
DECLARE_DO_FUN(do_entrance);
DECLARE_DO_FUN(do_fleshcraft);
DECLARE_DO_FUN(do_quills);
DECLARE_DO_FUN(do_perception);
DECLARE_DO_FUN(do_jawlock);
DECLARE_DO_FUN(do_rend);
DECLARE_DO_FUN(do_hardskin);
DECLARE_DO_FUN(do_slam);
DECLARE_DO_FUN(do_roar);
DECLARE_DO_FUN(do_shred);

DECLARE_DO_FUN(do_talons);
DECLARE_DO_FUN(do_devour);
DECLARE_DO_FUN(do_inferno);
DECLARE_DO_FUN(do_wall);
DECLARE_DO_FUN(do_facade);
DECLARE_DO_FUN(do_baal);
DECLARE_DO_FUN(do_vobj);
DECLARE_DO_FUN(do_dragonform);
DECLARE_DO_FUN(do_bloodwater);
DECLARE_DO_FUN(do_gourge);
DECLARE_DO_FUN(do_sharpen);
DECLARE_DO_FUN(do_dub);
DECLARE_DO_FUN(do_taste);
DECLARE_DO_FUN(do_assassinate);
DECLARE_DO_FUN(do_offer);
DECLARE_DO_FUN(do_gate);
DECLARE_DO_FUN(do_pigeon);
DECLARE_DO_FUN(do_bloodagony);
DECLARE_DO_FUN(do_decay);
DECLARE_DO_FUN(do_tell);
DECLARE_DO_FUN(do_theft);
DECLARE_DO_FUN(do_throw);
DECLARE_DO_FUN(do_tie);
DECLARE_DO_FUN(do_time);
DECLARE_DO_FUN(do_title);
DECLARE_DO_FUN(do_token);
DECLARE_DO_FUN(do_totems);
DECLARE_DO_FUN(do_track);
DECLARE_DO_FUN(do_tradition);
DECLARE_DO_FUN(do_train);
DECLARE_DO_FUN(do_transfer);
DECLARE_DO_FUN(do_transport);
DECLARE_DO_FUN(do_classportal);
DECLARE_DO_FUN(do_travel);
DECLARE_DO_FUN(do_ztravel);
DECLARE_DO_FUN(do_trust);
DECLARE_DO_FUN(do_turn);
DECLARE_DO_FUN(do_twist);
DECLARE_DO_FUN(do_typo);
DECLARE_DO_FUN(do_unlock);
DECLARE_DO_FUN(do_unpolymorph);
DECLARE_DO_FUN(do_untie);
DECLARE_DO_FUN(do_unwerewolf);
DECLARE_DO_FUN(do_up);
DECLARE_DO_FUN(do_wiznet);
DECLARE_DO_FUN(do_upkeep);
DECLARE_DO_FUN(do_users);
DECLARE_DO_FUN(do_vampire);
DECLARE_DO_FUN(do_vamptalk);
DECLARE_DO_FUN(do_tail);
DECLARE_DO_FUN(do_hooves);
DECLARE_DO_FUN(do_magetalk);
DECLARE_DO_FUN(do_version);
DECLARE_DO_FUN(do_visible);
DECLARE_DO_FUN(do_voodoo);
DECLARE_DO_FUN(do_web);
DECLARE_DO_FUN(do_wake);
DECLARE_DO_FUN(do_watcher);
DECLARE_DO_FUN(do_watching);
DECLARE_DO_FUN(do_weaponform);
DECLARE_DO_FUN(do_wear);
DECLARE_DO_FUN(do_wearaffect);
DECLARE_DO_FUN(do_werewolf);
DECLARE_DO_FUN(do_west);
DECLARE_DO_FUN(do_where);
DECLARE_DO_FUN(do_whisper);
DECLARE_DO_FUN(do_who);
DECLARE_DO_FUN(do_wings);
DECLARE_DO_FUN(do_wizhelp);
DECLARE_DO_FUN(do_linkdead);
DECLARE_DO_FUN(do_wizlist);
DECLARE_DO_FUN(do_weapset);
DECLARE_DO_FUN(do_closemud);
DECLARE_DO_FUN(do_wizlock);
DECLARE_DO_FUN(do_xemote);
DECLARE_DO_FUN(do_xsocials);
DECLARE_DO_FUN(do_yell);
DECLARE_DO_FUN(do_zap);
DECLARE_DO_FUN(do_zuloform);
DECLARE_DO_FUN(do_demonform);
DECLARE_DO_FUN(do_beckon);
DECLARE_DO_FUN(do_illusion);
DECLARE_DO_FUN(do_cserpent);
DECLARE_DO_FUN(do_scales);
DECLARE_DO_FUN(do_servant);
DECLARE_DO_FUN(do_contraception);
DECLARE_DO_FUN(do_relearn);

// New shit added by Hutoshi
DECLARE_DO_FUN(do_sigil);
DECLARE_DO_FUN(do_cprime);

// Akurei's things
DECLARE_DO_FUN( do_outerexits   );
DECLARE_DO_FUN( do_enterexits   );
DECLARE_DO_FUN( do_removeexit   );
DECLARE_DO_FUN( do_removeexits  );

