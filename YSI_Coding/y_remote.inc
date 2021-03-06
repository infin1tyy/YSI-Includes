#if defined _INC_y_remote
	#endinput
#endif
#define _INC_y_remote

/**
 * <library name="y_remote">
 * <section>
 *  Description
 * </section>
 *  Wrapper for "__CallRemoteFunction".  Enforces some features like no returns
 *  and arrays being followed by their length.
 * <section>
 *  Version
 * </section>
 *  1.0
 * <section>
 *  Macros
 * </section><ul>
 *  <symbol name="remotefunc">Define a function to be called remotely (use like "stock").</symbol>
 *  <symbol name="broadcastfunc">Call the function, but in all scripts.</symbol>
 *  <symbol name="localfunc">Call the function by name, not address, in the current script.</symbol>
 * </ul><section>Compile options</section><ul>
 *  <symbol name="YSI_NO_MASTER">Disable all knowledge of other scripts.</symbol>
 * </ul>
 * </library>
 *//** *//*
Legal:
	Version: MPL 1.1
	
	The contents of this file are subject to the Mozilla Public License Version 
	1.1 the "License"; you may not use this file except in compliance with 
	the License. You may obtain a copy of the License at 
	http://www.mozilla.org/MPL/
	
	Software distributed under the License is distributed on an "AS IS" basis,
	WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
	for the specific language governing rights and limitations under the
	License.
	
	The Original Code is the YSI framework.
	
	The Initial Developer of the Original Code is Alex "Y_Less" Cole.
	Portions created by the Initial Developer are Copyright C 2011
	the Initial Developer. All Rights Reserved.

Contributors:
	Y_Less
	koolk
	JoeBullet/Google63
	g_aSlice/Slice
	Misiur
	samphunter
	tianmeta
	maddinat0r
	spacemud
	Crayder
	Dayvison
	Ahmad45123
	Zeex
	irinel1996
	Yiin-
	Chaprnks
	Konstantinos
	Masterchen09
	Southclaws
	PatchwerkQWER
	m0k1
	paulommu
	udan111

Thanks:
	JoeBullet/Google63 - Handy arbitrary ASM jump code using SCTRL.
	ZeeX - Very productive conversations.
	koolk - IsPlayerinAreaEx code.
	TheAlpha - Danish translation.
	breadfish - German translation.
	Fireburn - Dutch translation.
	yom - French translation.
	50p - Polish translation.
	Zamaroht - Spanish translation.
	Los - Portuguese translation.
	Dracoblue, sintax, mabako, Xtreme, other coders - Producing other modes for
		me to strive to better.
	Pixels^ - Running XScripters where the idea was born.
	Matite - Pestering me to release it and using it.

Very special thanks to:
	Thiadmer - PAWN, whose limits continue to amaze me!
	Kye/Kalcor - SA:MP.
	SA:MP Team past, present and future - SA:MP.

Optional plugins:
	Gamer_Z - GPS.
	Incognito - Streamer.
	Me - sscanf2, fixes2, Whirlpool.
*/

#include "..\YSI_Internal\y_version"

#include "..\YSI_Internal\y_shortfunc"

#include "..\YSI_Core\y_utils"

// This VERY LOOSELY uses the master system to figure out if it should even
// bother with complex remote calls.  However, it isn't fully bound by the
// whole system because it's rules are different (and will work regardless).
#if defined YSI_NO_MASTER
	#define remotefunc stock
	#define localfunc%0(%1) (%0(%1))
	#define broadcastfunc%0(%1) (%0(%1))
	#define targetfunc%3<%2>%0(%1) (%3%0(%1))
#else	
	// IMPORTANT NOTE!  To avoid polluting the global name space (any more that
	// is), "y_inline" uses @Ru, @Rv, @Rw, @Rx and @Ry, as both files are low
	// usage. "y_testing" uses @Rf-@Ro.  "y_phone" uses @R0-@R9 and @RA-@RF
	// This now also uses @RH
	
	// This uses "stock" as it may be called from another script.
	//#define remotefunc%0(%1) stock%0_(%1)return J@?(W@(_:@Rz:#%0@##,_YM@CR:%1,,)):%0(%1);//%0@(__m);public %0@(__m)if(__m==-1||__m==_@)%0();%0(%1)
	#define remotefunc%0(%1) stock%0_(%1)if(@RN:@RO:%0:J@)return(W@(_:@Rz:#%0@##,_YM@CR:%1,,));else return(%0(%1));
	
	#define @RN:@RO:%9void:%0:J@)return(%2);else%8(%3); J@)(%2);else(%3);
	#define @RO:%0:J@) J@)
	
	// One macro to rule them all.  Instead of doing the parameter validation
	// and extraction multiple times, we just do it once and copy the values
	// when we're done with them all.  The macro is designed to be almost
	// entirely right if the function has no parameters.
	#define _YM@CR:%0,%1) @Ra:@Rb:@Rc:##|||%0|||%1)
	
	#define @Ra:@Rb:@Rc:#%0#%1|||%3[%4]%9|||%5,%6) @Rd:@Re:#%0#%1|||%3|||%5,%6)
	
	#define @Rd:@Re:#%0#%1|||%2string:%3|||%5,%6) @Ra:@Rb:@Rc:#%0s#%1,%3|||%5|||%6)
	#define @Re:#%0#%1|||%3|||%5,%6,%7) @Ra:@Rb:@Rc:#%0a#%1,%3|||%5|||%6,%7)
	
	#define @Rb:@Rc:#%0#%1|||%3|||%5,%6) @Ra:@Rb:@Rc:#%0i#%1,%3|||%5|||%6)
	
	#define @Rc:#%0#,%1||||||));%7(%2(%3)); @RH:@RI:@RJ:#%0#,%1));%7(%2(%3));
	
	// Detect "void:"
	#define @RH:@RI:@RJ:#%0#,%1));%7(%9void:%2(%3)); #%0i,%1,I@));%7(%2(%1));%2@(%3,__m);public %2@(%3,__m)if(__m==-1||__m==_@)%2(%1);%2(%3)
	// Detect "string:"
	#define @RI:@RJ:#%0#,%1));%7(%9string:%2(%3)); #%0i,%1,I@),F@());%7(%2(%1));%2@(%3,__m);public %2@(%3,__m)if(__m==-1||__m==_@)R@(%2(%1));%2(%3)
	// Detect other
	#define @RJ:#%0#,%1));%7(%2(%3)); #%0i,%1,I@),V@(8,YSIM_RETURN));%7(%2(%1));%2@(%3,__m);public %2@(%3,__m)if(__m==-1||__m==_@)X@(%2(%1));%2(%3)
	
	// Zero parameters.
	#define @Rz:#%0##,_YM@CR:,,));%7(%2()); #%0,@RK:@RL:@RM:#i,I@));%7(%2());
	// Detect "void:"
	#define @RK:@RL:@RM:#i,I@));%7(%9void:%2()); #i,I@));%7(%2());%2@(__m);public %2@(__m)if(__m==-1||__m==_@)%2();%2()
	// Detect "string:"
	#define @RL:@RM:#i,I@));%7(%9string:%2()); #i,I@),F@());%7(%2());%2@(__m);public %2@(__m)if(__m==-1||__m==_@)R@(%2());%2()
	// Detect other
	#define @RM:#i,I@));%7(%2()); #i,I@),V@(8,YSIM_RETURN));%7(%2());%2@(__m);public %2@(__m)if(__m==-1||__m==_@)X@(%2());%2()
	
	#define localfunc%0(%1) (J@=0,I@=-1,%0_(%1))
	#define broadcastfunc%0(%1) (J@=1,I@=-1,%0_(%1))
	// This supports "targetfunc<master> func()" and "targetfunc func<master>()".
	#define targetfunc%3<%2>%0(%1) (J@=1,I@=(%2),%3%0_(%1))
#endif

