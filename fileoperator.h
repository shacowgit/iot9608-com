#ifndef FILEOPERATOR_H
#define FILEOPERATOR_H
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <time.h>
#include <stdio.h>

#define F_SETFILE_LENGTH 110
#define F_ELECNAME    10  //
#define F_NEWID       20
#define F_NEWIDDATE   24
#define F_TOTALNUMBER 36
#define F_REALTIME    100
#define F_HISTORY     1600
#define TOTALCNT      10080
#define RECORDLEN     1431   //time 12 + data 1419
#define RECORDDATALEN 1419   // 
#define RECORDTIMELEN 12

#define V_A 3  //A相电压
#define V_B 8  //B相电压
#define V_C 13 //C相电压
#define C_A 18 //B相电流
#define C_B 23 //B相电流	5	23
#define C_C 28 //C相电流	5	28
#define Act_powerA 33 //A相有功	5	33
#define Act_powerB 38 //B相有功	5	38
#define Act_powerC 43 //C相有功	5	43
#define Act_powerT 48 //总有功	5	48
#define Rea_powerA 53 //A相无功	5	53
#define Rea_powerB 58 //B相无功	5	58
#define Rea_powerC 63 //C相无功	5	63
#define Rea_powerT 68 //总无功	5	68
#define App_powerA 73 //A相视在	5	73
#define App_powerB 78 //B相视在	5	78
#define App_powerC 83 //C相视在	5	83
#define App_powerT 88 //总视在	5	88
#define powerfac_A 93 //A相PF	3	93
#define powerfac_B 96 //B相PF	3	96
#define powerfac_C 99 //C相PF	3	99
#define powerfac_T 102 //总PF	3	102
#define frequency 105  //频率	3	105
#define V_fundA 108 //A相基波电压	5	108
#define V_fundB 113 //B相基波电压	5	113
#define V_fundC 118 //C相基波电压	5	118
#define C_fundA 123 //A相基波电流	5	123
#define C_fundB 128 //B相基波电流	5	128
#define C_fundC 133 //C相基波电流	5	133
#define V_har_coA2 138 //A相谐波电压含有率（2）	147	138
#define V_har_coA3 141 //A相谐波电压含有率（3）	147	138
#define V_har_coA4 144 //A相谐波电压含有率（4）	147	138
#define V_har_coA5 147 //A相谐波电压含有率（5）	147	138
#define V_har_coA6 150 //A相谐波电压含有率（6）	147	138
#define V_har_coA7 153 //A相谐波电压含有率（7）	147	138
#define V_har_coA8 156 //A相谐波电压含有率（8）	147	138
#define V_har_coA9 159 //A相谐波电压含有率（9）	147	138
#define V_har_coA10 162 //A相谐波电压含有率（10）	147	138
#define V_har_coA11 165 //A相谐波电压含有率（11）	147	138
#define V_har_coA12 168 //A相谐波电压含有率（12）	147	138
#define V_har_coA13 171 //A相谐波电压含有率（13）	147	138
#define V_har_coA14 174 //A相谐波电压含有率（14）	147	138
#define V_har_coA15 177 //A相谐波电压含有率（15）	147	138
#define V_har_coA16 180 //A相谐波电压含有率（16）	147	138
#define V_har_coA17 183 //A相谐波电压含有率（17）	147	138
#define V_har_coA18 186 //A相谐波电压含有率（18）	147	138
#define V_har_coA19 189 //A相谐波电压含有率（19）	147	138
#define V_har_coA20 192 //A相谐波电压含有率（20）	147	138
#define V_har_coA21 195 //A相谐波电压含有率（21）	147	138
#define V_har_coA22 198 //A相谐波电压含有率（22）	147	138
#define V_har_coA23 201 //A相谐波电压含有率（23）	147	138
#define V_har_coA24 204 //A相谐波电压含有率（24）	147	138
#define V_har_coA25 207 //A相谐波电压含有率（25）	147	138
#define V_har_coA26 210 //A相谐波电压含有率（26）	147	138
#define V_har_coA27 213 //A相谐波电压含有率（27）	147	138
#define V_har_coA28 216 //A相谐波电压含有率（28）	147	138
#define V_har_coA29 219 //A相谐波电压含有率（29）	147	138
#define V_har_coA30 222 //A相谐波电压含有率（30）	147	138
#define V_har_coA31 225 //A相谐波电压含有率（31）	147	138
#define V_har_coA32 228 //A相谐波电压含有率（32）	147	138
#define V_har_coA33 231 //A相谐波电压含有率（33）	147	138
#define V_har_coA34 234 //A相谐波电压含有率（34）	147	138
#define V_har_coA35 237 //A相谐波电压含有率（35）	147	138
#define V_har_coA36 240 //A相谐波电压含有率（36）	147	138
#define V_har_coA37 243 //A相谐波电压含有率（37）	147	138
#define V_har_coA38 246 //A相谐波电压含有率（38）	147	138
#define V_har_coA39 249 //A相谐波电压含有率（39）	147	138
#define V_har_coA40 252 //A相谐波电压含有率（40）	147	138
#define V_har_coA41 255 //A相谐波电压含有率（41）	147	138
#define V_har_coA42 258 //A相谐波电压含有率（42）	147	138
#define V_har_coA43 261 //A相谐波电压含有率（43）	147	138
#define V_har_coA44 264 //A相谐波电压含有率（44）	147	138
#define V_har_coA45 267 //A相谐波电压含有率（45）	147	138
#define V_har_coA46 270 //A相谐波电压含有率（46）	147	138
#define V_har_coA47 273 //A相谐波电压含有率（47）	147	138
#define V_har_coA48 276 //A相谐波电压含有率（48）	147	138
#define V_har_coA49 279 //A相谐波电压含有率（49）	147	138
#define V_har_coA50 282 //A相谐波电压含有率（50）	147	138
#define V_har_coB2 285 //B相谐波电压含有率（2）	147	138
#define V_har_coB3 288 //B相谐波电压含有率（3）	147	138
#define V_har_coB4 291 //B相谐波电压含有率（4）	147	138
#define V_har_coB5 294 //B相谐波电压含有率（5）	147	138
#define V_har_coB6 297 //B相谐波电压含有率（6）	147	138
#define V_har_coB7 300 //B相谐波电压含有率（7）	147	138
#define V_har_coB8 303 //B相谐波电压含有率（8）	147	138
#define V_har_coB9 306 //B相谐波电压含有率（9）	147	138
#define V_har_coB10 309 //B相谐波电压含有率（10）	147	138
#define V_har_coB11 312 //B相谐波电压含有率（11）	147	138
#define V_har_coB12 315 //B相谐波电压含有率（12）	147	138
#define V_har_coB13 318 //B相谐波电压含有率（13）	147	138
#define V_har_coB14 321 //B相谐波电压含有率（14）	147	138
#define V_har_coB15 324 //B相谐波电压含有率（15）	147	138
#define V_har_coB16 327 //B相谐波电压含有率（16）	147	138
#define V_har_coB17 330 //B相谐波电压含有率（17）	147	138
#define V_har_coB18 333 //B相谐波电压含有率（18）	147	138
#define V_har_coB19 336 //B相谐波电压含有率（19）	147	138
#define V_har_coB20 339 //B相谐波电压含有率（20）	147	138
#define V_har_coB21 342 //B相谐波电压含有率（21）	147	138
#define V_har_coB22 345 //B相谐波电压含有率（22）	147	138
#define V_har_coB23 348 //B相谐波电压含有率（23）	147	138
#define V_har_coB24 351 //B相谐波电压含有率（24）	147	138
#define V_har_coB25 354 //B相谐波电压含有率（25）	147	138
#define V_har_coB26 357 //B相谐波电压含有率（26）	147	138
#define V_har_coB27 360 //B相谐波电压含有率（27）	147	138
#define V_har_coB28 363 //B相谐波电压含有率（28）	147	138
#define V_har_coB29 366 //B相谐波电压含有率（29）	147	138
#define V_har_coB30 369 //B相谐波电压含有率（30）	147	138
#define V_har_coB31 372 //B相谐波电压含有率（31）	147	138
#define V_har_coB32 375 //B相谐波电压含有率（32）	147	138
#define V_har_coB33 378 //B相谐波电压含有率（33）	147	138
#define V_har_coB34 381 //B相谐波电压含有率（34）	147	138
#define V_har_coB35 384 //B相谐波电压含有率（35）	147	138
#define V_har_coB36 387 //B相谐波电压含有率（36）	147	138
#define V_har_coB37 390 //B相谐波电压含有率（37）	147	138
#define V_har_coB38 393 //B相谐波电压含有率（38）	147	138
#define V_har_coB39 396 //B相谐波电压含有率（39）	147	138
#define V_har_coB40 399 //B相谐波电压含有率（40）	147	138
#define V_har_coB41 402 //B相谐波电压含有率（41）	147	138
#define V_har_coB42 405 //B相谐波电压含有率（42）	147	138
#define V_har_coB43 408 //B相谐波电压含有率（43）	147	138
#define V_har_coB44 411 //B相谐波电压含有率（44）	147	138
#define V_har_coB45 414 //B相谐波电压含有率（45）	147	138
#define V_har_coB46 417 //B相谐波电压含有率（46）	147	138
#define V_har_coB47 420 //B相谐波电压含有率（47）	147	138
#define V_har_coB48 423 //B相谐波电压含有率（48）	147	138
#define V_har_coB49 426 //B相谐波电压含有率（49）	147	138
#define V_har_coB50 429 //B相谐波电压含有率（50）	147	138
#define V_har_coC2 432 //C相谐波电压含有率（2）	147	138
#define V_har_coC3 435 //C相谐波电压含有率（3）	147	138
#define V_har_coC4 438 //C相谐波电压含有率（4）	147	138
#define V_har_coC5 441 //C相谐波电压含有率（5）	147	138
#define V_har_coC6 444 //C相谐波电压含有率（6）	147	138
#define V_har_coC7 447 //C相谐波电压含有率（7）	147	138
#define V_har_coC8 450 //C相谐波电压含有率（8）	147	138
#define V_har_coC9 453 //C相谐波电压含有率（9）	147	138
#define V_har_coC10 456 //C相谐波电压含有率（10）	147	138
#define V_har_coC11 459 //C相谐波电压含有率（11）	147	138
#define V_har_coC12 462 //C相谐波电压含有率（12）	147	138
#define V_har_coC13 465 //C相谐波电压含有率（13）	147	138
#define V_har_coC14 468 //C相谐波电压含有率（14）	147	138
#define V_har_coC15 471 //C相谐波电压含有率（15）	147	138
#define V_har_coC16 474 //C相谐波电压含有率（16）	147	138
#define V_har_coC17 477 //C相谐波电压含有率（17）	147	138
#define V_har_coC18 480 //C相谐波电压含有率（18）	147	138
#define V_har_coC19 483 //C相谐波电压含有率（19）	147	138
#define V_har_coC20 486 //C相谐波电压含有率（20）	147	138
#define V_har_coC21 489 //C相谐波电压含有率（21）	147	138
#define V_har_coC22 492 //C相谐波电压含有率（22）	147	138
#define V_har_coC23 495 //C相谐波电压含有率（23）	147	138
#define V_har_coC24 498 //C相谐波电压含有率（24）	147	138
#define V_har_coC25 501 //C相谐波电压含有率（25）	147	138
#define V_har_coC26 504 //C相谐波电压含有率（26）	147	138
#define V_har_coC27 507 //C相谐波电压含有率（27）	147	138
#define V_har_coC28 510 //C相谐波电压含有率（28）	147	138
#define V_har_coC29 513 //C相谐波电压含有率（29）	147	138
#define V_har_coC30 516 //C相谐波电压含有率（30）	147	138
#define V_har_coC31 519 //C相谐波电压含有率（31）	147	138
#define V_har_coC32 522 //C相谐波电压含有率（32）	147	138
#define V_har_coC33 525 //C相谐波电压含有率（33）	147	138
#define V_har_coC34 528 //C相谐波电压含有率（34）	147	138
#define V_har_coC35 531 //C相谐波电压含有率（35）	147	138
#define V_har_coC36 534 //C相谐波电压含有率（36）	147	138
#define V_har_coC37 537 //C相谐波电压含有率（37）	147	138
#define V_har_coC38 540 //C相谐波电压含有率（38）	147	138
#define V_har_coC39 543 //C相谐波电压含有率（39）	147	138
#define V_har_coC40 546 //C相谐波电压含有率（40）	147	138
#define V_har_coC41 549 //C相谐波电压含有率（41）	147	138
#define V_har_coC42 552 //C相谐波电压含有率（42）	147	138
#define V_har_coC43 555 //C相谐波电压含有率（43）	147	138
#define V_har_coC44 558 //C相谐波电压含有率（44）	147	138
#define V_har_coC45 561 //C相谐波电压含有率（45）	147	138
#define V_har_coC46 564 //C相谐波电压含有率（46）	147	138
#define V_har_coC47 567 //C相谐波电压含有率（47）	147	138
#define V_har_coC48 570 //C相谐波电压含有率（48）	147	138
#define V_har_coC49 573 //C相谐波电压含有率（49）	147	138
#define V_har_coC50 576 //C相谐波电压含有率（50）	147	138
#define C_harA2 579 //A相谐波电流（2）	147	138
#define C_harA3 584 //A相谐波电流（3）	147	138
#define C_harA4 589 //A相谐波电流（4）	147	138
#define C_harA5 594 //A相谐波电流（5）	147	138
#define C_harA6 599 //A相谐波电流（6）	147	138
#define C_harA7 604 //A相谐波电流（7）	147	138
#define C_harA8 609 //A相谐波电流（8）	147	138
#define C_harA9 614 //A相谐波电流（9）	147	138
#define C_harA10 619 //A相谐波电流（10）	147	138
#define C_harA11 624 //A相谐波电流（11）	147	138
#define C_harA12 629 //A相谐波电流（12）	147	138
#define C_harA13 634 //A相谐波电流（13）	147	138
#define C_harA14 639 //A相谐波电流（14）	147	138
#define C_harA15 644 //A相谐波电流（15）	147	138
#define C_harA16 649 //A相谐波电流（16）	147	138
#define C_harA17 654 //A相谐波电流（17）	147	138
#define C_harA18 659 //A相谐波电流（18）	147	138
#define C_harA19 664 //A相谐波电流（19）	147	138
#define C_harA20 669 //A相谐波电流（20）	147	138
#define C_harA21 674 //A相谐波电流（21）	147	138
#define C_harA22 679 //A相谐波电流（22）	147	138
#define C_harA23 684 //A相谐波电流（23）	147	138
#define C_harA24 689 //A相谐波电流（24）	147	138
#define C_harA25 694 //A相谐波电流（25）	147	138
#define C_harA26 699 //A相谐波电流（26）	147	138
#define C_harA27 704 //A相谐波电流（27）	147	138
#define C_harA28 709 //A相谐波电流（28）	147	138
#define C_harA29 714 //A相谐波电流（29）	147	138
#define C_harA30 719 //A相谐波电流（30）	147	138
#define C_harA31 724 //A相谐波电流（31）	147	138
#define C_harA32 729 //A相谐波电流（32）	147	138
#define C_harA33 734 //A相谐波电流（33）	147	138
#define C_harA34 739 //A相谐波电流（34）	147	138
#define C_harA35 744 //A相谐波电流（35）	147	138
#define C_harA36 749 //A相谐波电流（36）	147	138
#define C_harA37 754 //A相谐波电流（37）	147	138
#define C_harA38 759 //A相谐波电流（38）	147	138
#define C_harA39 764 //A相谐波电流（39）	147	138
#define C_harA40 769 //A相谐波电流（40）	147	138
#define C_harA41 774 //A相谐波电流（41）	147	138
#define C_harA42 779 //A相谐波电流（42）	147	138
#define C_harA43 784 //A相谐波电流（43）	147	138
#define C_harA44 789 //A相谐波电流（44）	147	138
#define C_harA45 794 //A相谐波电流（45）	147	138
#define C_harA46 799 //A相谐波电流（46）	147	138
#define C_harA47 804 //A相谐波电流（47）	147	138
#define C_harA48 809 //A相谐波电流（48）	147	138
#define C_harA49 814 //A相谐波电流（49）	147	138
#define C_harA50 819 //A相谐波电流（50）	147	138
#define C_harB2 824 //B相谐波电流（2）	147	138
#define C_harB3 829 //B相谐波电流（3）	147	138
#define C_harB4 834 //B相谐波电流（4）	147	138
#define C_harB5 839 //B相谐波电流（5）	147	138
#define C_harB6 844 //B相谐波电流（6）	147	138
#define C_harB7 849 //B相谐波电流（7）	147	138
#define C_harB8 854 //B相谐波电流（8）	147	138
#define C_harB9 859 //B相谐波电流（9）	147	138
#define C_harB10 864 //B相谐波电流（10）	147	138
#define C_harB11 869 //B相谐波电流（11）	147	138
#define C_harB12 874 //B相谐波电流（12）	147	138
#define C_harB13 879 //B相谐波电流（13）	147	138
#define C_harB14 884 //B相谐波电流（14）	147	138
#define C_harB15 889 //B相谐波电流（15）	147	138
#define C_harB16 894 //B相谐波电流（16）	147	138
#define C_harB17 899 //B相谐波电流（17）	147	138
#define C_harB18 904 //B相谐波电流（18）	147	138
#define C_harB19 909 //B相谐波电流（19）	147	138
#define C_harB20 914 //B相谐波电流（20）	147	138
#define C_harB21 919 //B相谐波电流（21）	147	138
#define C_harB22 924 //B相谐波电流（22）	147	138
#define C_harB23 929 //B相谐波电流（23）	147	138
#define C_harB24 934 //B相谐波电流（24）	147	138
#define C_harB25 939 //B相谐波电流（25）	147	138
#define C_harB26 944 //B相谐波电流（26）	147	138
#define C_harB27 949 //B相谐波电流（27）	147	138
#define C_harB28 954 //B相谐波电流（28）	147	138
#define C_harB29 959 //B相谐波电流（29）	147	138
#define C_harB30 964 //B相谐波电流（30）	147	138
#define C_harB31 969 //B相谐波电流（31）	147	138
#define C_harB32 974 //B相谐波电流（32）	147	138
#define C_harB33 979 //B相谐波电流（33）	147	138
#define C_harB34 984 //B相谐波电流（34）	147	138
#define C_harB35 989 //B相谐波电流（35）	147	138
#define C_harB36 994 //B相谐波电流（36）	147	138
#define C_harB37 999 //B相谐波电流（37）	147	138
#define C_harB38 1004 //B相谐波电流（38）	147	138
#define C_harB39 1009 //B相谐波电流（39）	147	138
#define C_harB40 1014 //B相谐波电流（40）	147	138
#define C_harB41 1019 //B相谐波电流（41）	147	138
#define C_harB42 1024 //B相谐波电流（42）	147	138
#define C_harB43 1029 //B相谐波电流（43）	147	138
#define C_harB44 1034 //B相谐波电流（44）	147	138
#define C_harB45 1039 //B相谐波电流（45）	147	138
#define C_harB46 1044 //B相谐波电流（46）	147	138
#define C_harB47 1049 //B相谐波电流（47）	147	138
#define C_harB48 1054 //B相谐波电流（48）	147	138
#define C_harB49 1059 //B相谐波电流（49）	147	138
#define C_harB50 1064 //B相谐波电流（50）	147	138
#define C_harC2 1069 //C相谐波电流（2）	147	138
#define C_harC3 1074 //C相谐波电流（3）	147	138
#define C_harC4 1079 //C相谐波电流（4）	147	138
#define C_harC5 1084 //C相谐波电流（5）	147	138
#define C_harC6 1089 //C相谐波电流（6）	147	138
#define C_harC7 1094 //C相谐波电流（7）	147	138
#define C_harC8 1099 //C相谐波电流（8）	147	138
#define C_harC9 1104 //C相谐波电流（9）	147	138
#define C_harC10 1109 //C相谐波电流（10）	147	138
#define C_harC11 1114 //C相谐波电流（11）	147	138
#define C_harC12 1119 //C相谐波电流（12）	147	138
#define C_harC13 1124 //C相谐波电流（13）	147	138
#define C_harC14 1129 //C相谐波电流（14）	147	138
#define C_harC15 1134 //C相谐波电流（15）	147	138
#define C_harC16 1139 //C相谐波电流（16）	147	138
#define C_harC17 1144 //C相谐波电流（17）	147	138
#define C_harC18 1149 //C相谐波电流（18）	147	138
#define C_harC19 1154 //C相谐波电流（19）	147	138
#define C_harC20 1159 //C相谐波电流（20）	147	138
#define C_harC21 1164 //C相谐波电流（21）	147	138
#define C_harC22 1169 //C相谐波电流（22）	147	138
#define C_harC23 1174 //C相谐波电流（23）	147	138
#define C_harC24 1179 //C相谐波电流（24）	147	138
#define C_harC25 1184 //C相谐波电流（25）	147	138
#define C_harC26 1189 //C相谐波电流（26）	147	138
#define C_harC27 1194 //C相谐波电流（27）	147	138
#define C_harC28 1199 //C相谐波电流（28）	147	138
#define C_harC29 1204 //C相谐波电流（29）	147	138
#define C_harC30 1209 //C相谐波电流（30）	147	138
#define C_harC31 1214 //C相谐波电流（31）	147	138
#define C_harC32 1219 //C相谐波电流（32）	147	138
#define C_harC33 1224 //C相谐波电流（33）	147	138
#define C_harC34 1229 //C相谐波电流（34）	147	138
#define C_harC35 1234 //C相谐波电流（35）	147	138
#define C_harC36 1239 //C相谐波电流（36）	147	138
#define C_harC37 1244 //C相谐波电流（37）	147	138
#define C_harC38 1249 //C相谐波电流（38）	147	138
#define C_harC39 1254 //C相谐波电流（39）	147	138
#define C_harC40 1259 //C相谐波电流（40）	147	138
#define C_harC41 1264 //C相谐波电流（41）	147	138
#define C_harC42 1269 //C相谐波电流（42）	147	138
#define C_harC43 1274 //C相谐波电流（43）	147	138
#define C_harC44 1279 //C相谐波电流（44）	147	138
#define C_harC45 1284 //C相谐波电流（45）	147	138
#define C_harC46 1289 //C相谐波电流（46）	147	138
#define C_harC47 1294 //C相谐波电流（47）	147	138
#define C_harC48 1299 //C相谐波电流（48）	147	138
#define C_harC49 1304 //C相谐波电流（49）	147	138
#define C_harC50 1309 //C相谐波电流（50）	147	138
#define V_wav_disA 1314 //A相电压谐波畸变率	3	1314
#define V_wav_disB 1317 //B相电压谐波畸变率	3	1317
#define V_wav_disC 1320 //C相电压谐波畸变率	3	1320
#define C_wav_disA 1323 //A相电流谐波畸变率	3	1323
#define C_wav_disB 1326 //B相电流谐波畸变率	3	1326
#define C_wav_disC 1329 //C相电流谐波畸变率	3	1329
#define P_act_powA 1332 //A相正向有功总电能	5	1332
#define P_act_powB 1337 //B相正向有功总电能	5	1337
#define P_act_powC 1342 //C相正向有功总电能	5	1342
#define P_act_powT 1347 //总正向有功总电能	5	1347
#define P_rea_powA 1352 //A相正向无功总电能	5	1352
#define P_rea_powB 1357 //B相正向无功总电能	5	1357
#define P_rea_powC 1362 //C相正向无功总电能	5	1362
#define P_rea_powT 1367 //总正向无功总电能	5	1367
#define R_act_powA 1372 //A相反向有功总电能	5	1372
#define R_act_powB 1377 //B相反向有功总电能	5	1377
#define R_act_powC 1382 //C相反向有功总电能	5	1382
#define R_act_powT 1387 //总反向有功总电能	5	1387
#define R_rea_powA 1392 //A相反向无功总电能	5	1392
#define R_rea_powB 1397 //B相反向无功总电能	5	1397
#define R_rea_powC 1402 //C相反向无功总电能	5	1402
#define R_rea_powT 1407 //总反向无功总电能	5	1407
#define V_imdegree 1412 //电压不平衡度	3	1412
#define C_imdegree 1415 //电流不平衡度	3	1415
#define dataend    1417 //the end

extern int chartoi(char buf[],int offset, int len);                                         //bytes to int
extern double getrealvalue(char valuebuf[],int valueoff,int len,int fnum);                  //
extern int insertsavedatar(char savedata[],int saveoff, double value,int len,int fnum);
extern int insertsavedata(char valuebuf[],char savedata[],int valueoff,int saveoff,int num);
extern int savedatainit(char savedata[]);
extern int fgetrtdata(char fname[],char databuf[]);
extern int fgethddata(char fname[],char databuf[]);
extern int fgetsetdata(char databuf[]);
extern int fsetsetdata(char databuf[]);
extern int wrfile(char fname[],char databuf[]);
extern int fgetworktype();   //1:timing;2:ask;other:tran
extern int fgetsendspan();
extern int fgethisdata(char fname[],int index,char databuf[]);
#endif

