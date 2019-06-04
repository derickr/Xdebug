--TEST--
Test for bug #1486: Crash on ZEND_SWITCH_LONG / ZEND_SWITCH_STRING (>= PHP 7.2, <= PHP 7.2.13, opcache)
--SKIPIF--
<?php
require 'tests/utils.inc';
check_reqs('PHP >= 7.2,<= 7.2.13; opcache');
?>
--FILE--
<?php
include 'dump-branch-coverage.inc';

$foo = [38, 1, 17, 23];
xdebug_start_code_coverage(XDEBUG_CC_UNUSED | XDEBUG_CC_DEAD_CODE | XDEBUG_CC_BRANCH_CHECK );
include dirname( __FILE__ ) . '/bug01486-001.inc';
$cc = xdebug_get_code_coverage();
dump_branch_coverage($cc);
xdebug_stop_code_coverage();
?>
--EXPECTF--
foo 38
foo 1
foo 17
foo 23
{main}
- branches
  - 00; OP: 00-01; line: 02-02 HIT; out1: 02 HIT; out2: 366  X 
  - 02; OP: 02-02; line: 02-02 HIT; out1: 03 HIT; out2: 366 HIT
  - 03; OP: 03-05; line: 02-03 HIT; out1: 86  X ; out2: 93  X ; out3: 100  X ; out4: 107  X ; out5: 114  X ; out6: 121  X ; out7: 128  X ; out8: 135  X ; out9: 142  X ; out10: 149  X ; out11: 156  X ; out12: 163  X ; out13: 170  X ; out14: 177  X ; out15: 184  X ; out16: 191  X ; out17: 198  X ; out18: 205  X ; out19: 212  X ; out20: 219  X ; out21: 226  X ; out22: 233  X ; out23: 240  X ; out24: 247  X ; out25: 254  X ; out26: 261  X ; out27: 268  X ; out28: 275  X ; out29: 282  X ; out30: 289  X ; out31: 296  X ; out32: 303  X ; out33: 310  X ; out34: 317  X ; out35: 324  X ; out36: 331  X ; out37: 338  X ; out38: 345  X ; out39: 352  X ; out40: 359  X ; out41: 365  X ; out42: 06 HIT
  - 06; OP: 06-07; line: 04-04 HIT; out1: 08 HIT; out2: 86 HIT
  - 08; OP: 08-09; line: 05-05 HIT; out1: 10 HIT; out2: 93  X 
  - 10; OP: 10-11; line: 06-06 HIT; out1: 12 HIT; out2: 100  X 
  - 12; OP: 12-13; line: 07-07 HIT; out1: 14 HIT; out2: 107  X 
  - 14; OP: 14-15; line: 08-08 HIT; out1: 16 HIT; out2: 114  X 
  - 16; OP: 16-17; line: 09-09 HIT; out1: 18 HIT; out2: 121  X 
  - 18; OP: 18-19; line: 10-10 HIT; out1: 20 HIT; out2: 128  X 
  - 20; OP: 20-21; line: 11-11 HIT; out1: 22 HIT; out2: 135  X 
  - 22; OP: 22-23; line: 12-12 HIT; out1: 24 HIT; out2: 142  X 
  - 24; OP: 24-25; line: 13-13 HIT; out1: 26 HIT; out2: 149  X 
  - 26; OP: 26-27; line: 14-14 HIT; out1: 28 HIT; out2: 156  X 
  - 28; OP: 28-29; line: 15-15 HIT; out1: 30 HIT; out2: 163  X 
  - 30; OP: 30-31; line: 16-16 HIT; out1: 32 HIT; out2: 170  X 
  - 32; OP: 32-33; line: 17-17 HIT; out1: 34 HIT; out2: 177  X 
  - 34; OP: 34-35; line: 18-18 HIT; out1: 36 HIT; out2: 184  X 
  - 36; OP: 36-37; line: 19-19 HIT; out1: 38 HIT; out2: 191  X 
  - 38; OP: 38-39; line: 20-20 HIT; out1: 40 HIT; out2: 198 HIT
  - 40; OP: 40-41; line: 21-21 HIT; out1: 42 HIT; out2: 205  X 
  - 42; OP: 42-43; line: 22-22 HIT; out1: 44 HIT; out2: 212  X 
  - 44; OP: 44-45; line: 23-23 HIT; out1: 46 HIT; out2: 219  X 
  - 46; OP: 46-47; line: 24-24 HIT; out1: 48 HIT; out2: 226  X 
  - 48; OP: 48-49; line: 25-25 HIT; out1: 50 HIT; out2: 233  X 
  - 50; OP: 50-51; line: 26-26 HIT; out1: 52 HIT; out2: 240 HIT
  - 52; OP: 52-53; line: 27-27 HIT; out1: 54 HIT; out2: 247  X 
  - 54; OP: 54-55; line: 28-28 HIT; out1: 56 HIT; out2: 254  X 
  - 56; OP: 56-57; line: 29-29 HIT; out1: 58 HIT; out2: 261  X 
  - 58; OP: 58-59; line: 30-30 HIT; out1: 60 HIT; out2: 268  X 
  - 60; OP: 60-61; line: 31-31 HIT; out1: 62 HIT; out2: 275  X 
  - 62; OP: 62-63; line: 32-32 HIT; out1: 64 HIT; out2: 282  X 
  - 64; OP: 64-65; line: 33-33 HIT; out1: 66 HIT; out2: 289  X 
  - 66; OP: 66-67; line: 34-34 HIT; out1: 68 HIT; out2: 296  X 
  - 68; OP: 68-69; line: 35-35 HIT; out1: 70 HIT; out2: 303  X 
  - 70; OP: 70-71; line: 36-36 HIT; out1: 72 HIT; out2: 310  X 
  - 72; OP: 72-73; line: 37-37 HIT; out1: 74 HIT; out2: 317  X 
  - 74; OP: 74-75; line: 38-38 HIT; out1: 76 HIT; out2: 324  X 
  - 76; OP: 76-77; line: 39-39 HIT; out1: 78 HIT; out2: 331  X 
  - 78; OP: 78-79; line: 40-40 HIT; out1: 80 HIT; out2: 338  X 
  - 80; OP: 80-81; line: 41-41 HIT; out1: 82  X ; out2: 345 HIT
  - 82; OP: 82-83; line: 42-42  X ; out1: 84  X ; out2: 352  X 
  - 84; OP: 84-85; line: 43-43  X ; out1: 02  X ; out2: 359  X 
  - 86; OP: 86-92; line: 04-04 HIT; out1: 02 HIT
  - 93; OP: 93-99; line: 05-05  X ; out1: 02  X 
  - 100; OP: 100-106; line: 06-06  X ; out1: 02  X 
  - 107; OP: 107-113; line: 07-07  X ; out1: 02  X 
  - 114; OP: 114-120; line: 08-08  X ; out1: 02  X 
  - 121; OP: 121-127; line: 09-09  X ; out1: 02  X 
  - 128; OP: 128-134; line: 10-10  X ; out1: 02  X 
  - 135; OP: 135-141; line: 11-11  X ; out1: 02  X 
  - 142; OP: 142-148; line: 12-12  X ; out1: 02  X 
  - 149; OP: 149-155; line: 13-13  X ; out1: 02  X 
  - 156; OP: 156-162; line: 14-14  X ; out1: 02  X 
  - 163; OP: 163-169; line: 15-15  X ; out1: 02  X 
  - 170; OP: 170-176; line: 16-16  X ; out1: 02  X 
  - 177; OP: 177-183; line: 17-17  X ; out1: 02  X 
  - 184; OP: 184-190; line: 18-18  X ; out1: 02  X 
  - 191; OP: 191-197; line: 19-19  X ; out1: 02  X 
  - 198; OP: 198-204; line: 20-20 HIT; out1: 02 HIT
  - 205; OP: 205-211; line: 21-21  X ; out1: 02  X 
  - 212; OP: 212-218; line: 22-22  X ; out1: 02  X 
  - 219; OP: 219-225; line: 23-23  X ; out1: 02  X 
  - 226; OP: 226-232; line: 24-24  X ; out1: 02  X 
  - 233; OP: 233-239; line: 25-25  X ; out1: 02  X 
  - 240; OP: 240-246; line: 26-26 HIT; out1: 02 HIT
  - 247; OP: 247-253; line: 27-27  X ; out1: 02  X 
  - 254; OP: 254-260; line: 28-28  X ; out1: 02  X 
  - 261; OP: 261-267; line: 29-29  X ; out1: 02  X 
  - 268; OP: 268-274; line: 30-30  X ; out1: 02  X 
  - 275; OP: 275-281; line: 31-31  X ; out1: 02  X 
  - 282; OP: 282-288; line: 32-32  X ; out1: 02  X 
  - 289; OP: 289-295; line: 33-33  X ; out1: 02  X 
  - 296; OP: 296-302; line: 34-34  X ; out1: 02  X 
  - 303; OP: 303-309; line: 35-35  X ; out1: 02  X 
  - 310; OP: 310-316; line: 36-36  X ; out1: 02  X 
  - 317; OP: 317-323; line: 37-37  X ; out1: 02  X 
  - 324; OP: 324-330; line: 38-38  X ; out1: 02  X 
  - 331; OP: 331-337; line: 39-39  X ; out1: 02  X 
  - 338; OP: 338-344; line: 40-40  X ; out1: 02  X 
  - 345; OP: 345-351; line: 41-41 HIT; out1: 02 HIT
  - 352; OP: 352-358; line: 42-42  X ; out1: 02  X 
  - 359; OP: 359-364; line: 43-43  X ; out1: 365  X 
  - 365; OP: 365-365; line: 43-43  X ; out1: 02  X 
  - 366; OP: 366-367; line: 43-46 HIT; out1: EX  X 
- paths
  - 0 2 3 86 2 366:  X 
  - 0 2 3 93 2 366:  X 
  - 0 2 3 100 2 366:  X 
  - 0 2 3 107 2 366:  X 
  - 0 2 3 114 2 366:  X 
  - 0 2 3 121 2 366:  X 
  - 0 2 3 128 2 366:  X 
  - 0 2 3 135 2 366:  X 
  - 0 2 3 142 2 366:  X 
  - 0 2 3 149 2 366:  X 
  - 0 2 3 156 2 366:  X 
  - 0 2 3 163 2 366:  X 
  - 0 2 3 170 2 366:  X 
  - 0 2 3 177 2 366:  X 
  - 0 2 3 184 2 366:  X 
  - 0 2 3 191 2 366:  X 
  - 0 2 3 198 2 366:  X 
  - 0 2 3 205 2 366:  X 
  - 0 2 3 212 2 366:  X 
  - 0 2 3 219 2 366:  X 
  - 0 2 3 226 2 366:  X 
  - 0 2 3 233 2 366:  X 
  - 0 2 3 240 2 366:  X 
  - 0 2 3 247 2 366:  X 
  - 0 2 3 254 2 366:  X 
  - 0 2 3 261 2 366:  X 
  - 0 2 3 268 2 366:  X 
  - 0 2 3 275 2 366:  X 
  - 0 2 3 282 2 366:  X 
  - 0 2 3 289 2 366:  X 
  - 0 2 3 296 2 366:  X 
  - 0 2 3 303 2 366:  X 
  - 0 2 3 310 2 366:  X 
  - 0 2 3 317 2 366:  X 
  - 0 2 3 324 2 366:  X 
  - 0 2 3 331 2 366:  X 
  - 0 2 3 338 2 366:  X 
  - 0 2 3 345 2 366:  X 
  - 0 2 3 352 2 366:  X 
  - 0 2 3 359 365 2 366:  X 
  - 0 2 3 365 2 366:  X 
  - 0 2 3 6 8 10 12 14 16 18 20 22 24 26 28 30 32 34 36 38 40 42 44 46 48 50 52 54 56 58 60 62 64 66 68 70 72 74 76 78 80 82 84 2 366:  X 
  - 0 2 3 6 8 10 12 14 16 18 20 22 24 26 28 30 32 34 36 38 40 42 44 46 48 50 52 54 56 58 60 62 64 66 68 70 72 74 76 78 80 82 84 359 365 2 366:  X 
  - 0 2 3 6 8 10 12 14 16 18 20 22 24 26 28 30 32 34 36 38 40 42 44 46 48 50 52 54 56 58 60 62 64 66 68 70 72 74 76 78 80 82 352 2 366:  X 
  - 0 2 3 6 8 10 12 14 16 18 20 22 24 26 28 30 32 34 36 38 40 42 44 46 48 50 52 54 56 58 60 62 64 66 68 70 72 74 76 78 80 345 2 366:  X 
  - 0 2 3 6 8 10 12 14 16 18 20 22 24 26 28 30 32 34 36 38 40 42 44 46 48 50 52 54 56 58 60 62 64 66 68 70 72 74 76 78 338 2 366:  X 
  - 0 2 3 6 8 10 12 14 16 18 20 22 24 26 28 30 32 34 36 38 40 42 44 46 48 50 52 54 56 58 60 62 64 66 68 70 72 74 76 331 2 366:  X 
  - 0 2 3 6 8 10 12 14 16 18 20 22 24 26 28 30 32 34 36 38 40 42 44 46 48 50 52 54 56 58 60 62 64 66 68 70 72 74 324 2 366:  X 
  - 0 2 3 6 8 10 12 14 16 18 20 22 24 26 28 30 32 34 36 38 40 42 44 46 48 50 52 54 56 58 60 62 64 66 68 70 72 317 2 366:  X 
  - 0 2 3 6 8 10 12 14 16 18 20 22 24 26 28 30 32 34 36 38 40 42 44 46 48 50 52 54 56 58 60 62 64 66 68 70 310 2 366:  X 
  - 0 2 3 6 8 10 12 14 16 18 20 22 24 26 28 30 32 34 36 38 40 42 44 46 48 50 52 54 56 58 60 62 64 66 68 303 2 366:  X 
  - 0 2 3 6 8 10 12 14 16 18 20 22 24 26 28 30 32 34 36 38 40 42 44 46 48 50 52 54 56 58 60 62 64 66 296 2 366:  X 
  - 0 2 3 6 8 10 12 14 16 18 20 22 24 26 28 30 32 34 36 38 40 42 44 46 48 50 52 54 56 58 60 62 64 289 2 366:  X 
  - 0 2 3 6 8 10 12 14 16 18 20 22 24 26 28 30 32 34 36 38 40 42 44 46 48 50 52 54 56 58 60 62 282 2 366:  X 
  - 0 2 3 6 8 10 12 14 16 18 20 22 24 26 28 30 32 34 36 38 40 42 44 46 48 50 52 54 56 58 60 275 2 366:  X 
  - 0 2 3 6 8 10 12 14 16 18 20 22 24 26 28 30 32 34 36 38 40 42 44 46 48 50 52 54 56 58 268 2 366:  X 
  - 0 2 3 6 8 10 12 14 16 18 20 22 24 26 28 30 32 34 36 38 40 42 44 46 48 50 52 54 56 261 2 366:  X 
  - 0 2 3 6 8 10 12 14 16 18 20 22 24 26 28 30 32 34 36 38 40 42 44 46 48 50 52 54 254 2 366:  X 
  - 0 2 3 6 8 10 12 14 16 18 20 22 24 26 28 30 32 34 36 38 40 42 44 46 48 50 52 247 2 366:  X 
  - 0 2 3 6 8 10 12 14 16 18 20 22 24 26 28 30 32 34 36 38 40 42 44 46 48 50 240 2 366:  X 
  - 0 2 3 6 8 10 12 14 16 18 20 22 24 26 28 30 32 34 36 38 40 42 44 46 48 233 2 366:  X 
  - 0 2 3 6 8 10 12 14 16 18 20 22 24 26 28 30 32 34 36 38 40 42 44 46 226 2 366:  X 
  - 0 2 3 6 8 10 12 14 16 18 20 22 24 26 28 30 32 34 36 38 40 42 44 219 2 366:  X 
  - 0 2 3 6 8 10 12 14 16 18 20 22 24 26 28 30 32 34 36 38 40 42 212 2 366:  X 
  - 0 2 3 6 8 10 12 14 16 18 20 22 24 26 28 30 32 34 36 38 40 205 2 366:  X 
  - 0 2 3 6 8 10 12 14 16 18 20 22 24 26 28 30 32 34 36 38 198 2 366:  X 
  - 0 2 3 6 8 10 12 14 16 18 20 22 24 26 28 30 32 34 36 191 2 366:  X 
  - 0 2 3 6 8 10 12 14 16 18 20 22 24 26 28 30 32 34 184 2 366:  X 
  - 0 2 3 6 8 10 12 14 16 18 20 22 24 26 28 30 32 177 2 366:  X 
  - 0 2 3 6 8 10 12 14 16 18 20 22 24 26 28 30 170 2 366:  X 
  - 0 2 3 6 8 10 12 14 16 18 20 22 24 26 28 163 2 366:  X 
  - 0 2 3 6 8 10 12 14 16 18 20 22 24 26 156 2 366:  X 
  - 0 2 3 6 8 10 12 14 16 18 20 22 24 149 2 366:  X 
  - 0 2 3 6 8 10 12 14 16 18 20 22 142 2 366:  X 
  - 0 2 3 6 8 10 12 14 16 18 20 135 2 366:  X 
  - 0 2 3 6 8 10 12 14 16 18 128 2 366:  X 
  - 0 2 3 6 8 10 12 14 16 121 2 366:  X 
  - 0 2 3 6 8 10 12 14 114 2 366:  X 
  - 0 2 3 6 8 10 12 107 2 366:  X 
  - 0 2 3 6 8 10 100 2 366:  X 
  - 0 2 3 6 8 93 2 366:  X 
  - 0 2 3 6 86 2 366:  X 
  - 0 2 366:  X 
  - 0 366:  X
