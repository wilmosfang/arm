;﻿;
;LED1-GPB5 根据原理图搜索，这里使用的天嵌开发板，不同板子不一样
;LED2-GPB6
;LED3-GPB7
;LED4-GPB8
;在ARM的汇编里 ‘;’ 代表注释

;0-On 低电位 
;1-Off 高电位
;

;GPBCON-0x56000010 GPB的控制寄存器地址，R/W
;GPBDAT-0x56000014 GPB的数据寄存器地址，R/W
;GPBUP -0x56000018 GPB的上拉电阻寄存器地址，R/W 0:开启，1:关闭
    

	AREA    RESET, CODE, READONLY
	;AREA 伪指令用于定义一个代表段或数据段，ARM汇编程序设计采用分段式设计，一个ARM源程序至少需要一个代码段，大的程序可以包含多个代码段及数据段
	;RESET 是代码段或数据段的名称
	;CODE 定义为代码段
	;READONLY 指定本段为只读，代码段的默认属性为只读
	
	ENTRY	;ENTRY伪指令用于指定程序的入口点,一个程序(可以包含多个源文件)中至少要有一个ENTRY,可以有多个ENTRY，但一个源文件中最多只有一个ENTRY
	

	CODE32
	;CODE32伪指令指示汇编编译器后面的指令为32位的ARM指令

	IMPORT CtrlLED ;IMPORT伪指令指示编译器当前的符号不是在本源文件中定义的，而是在其它源文件中定义的，在本源文件中可能引用该符号
	IMPORT CtrlBEEP ;CtrlLED CtrlBEEP 两个符号都是在另一个文件中定义的

	PRESERVE8 ;PRESERVE8伪指令指示当前文件保持堆栈为8字节对齐

	;关看门狗相关配置
	LDR		R0, =0x53000000 ;0x53000000是看门狗定时器的控制寄存器地址,LDR是将这个地址加载到R0寄存器中
	LDR		R1, =0        	;LDR将0这个立即数，加载到R1寄存器中
	STR		R1, [R0]		;STR将R1寄存器中的值(0)存到R0寄存器中地址所指代的寄存器中,在这里，总体来讲就是关闭了看门狗

	LDR		SP, =0x1000	;将堆栈指针指向0x1000的地址，为什么是0x1000呢？我人为的设定了IROM1的范围为 0x0-0x800,IRAM1的范围为0x800-0x1000,那么前0x800Byte的空间就用来存代码了，后面的0x800就用来放数据了，然而堆栈是从大地址往小地址移动，所以从0x1000开始存储数据，故将SP指向0x1000的初始位置

	BL 		CtrlLED 	;跳转到CtrlLED执行
	BL 		CtrlBEEP 	;跳转到CtrlBEEP执行

 
	END 		;汇编结束
