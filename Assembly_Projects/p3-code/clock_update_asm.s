.text
.global  set_tod_from_secs

## ENTRY POINT FOR REQUIRED FUNCTION
## first arg int TIME_OF_DAY_SEC in %rdi
## second arg pointer tod in %rsi
set_tod_from_secs:
        ## assembly instructions here
        cmpl $0, %edi           ## compare TIME_OF_DAY_SEC with 0
        jl .error1              ## jump to error case if 0 is greater
        cmpq $86400, %rdi       ## compare TIME_OF_DAY_SEC and 86400
        jg .error1              ## jump to error case is greater than 86400
        movb $0, 6(%rsi)        ## set tod.ispm = 0
        movq %rdi, %rax         ## move TIME_OF_DAY_SEC into %rax to prep for division
        cqto                    ## extend sign of rax to rdx
        movq $3600, %r8         ## move 3600 into the divisor register
      	movq $0, %rdx
      	idivq %r8               ## %rax / %r8 = quotient + rem
                                ## %rax == quotient(hours), %rdx == rem
        cmpq $12, %rax          ## compare if hours(%rax) is greater 12
        jle .AM                 ## jump ahead if 12 >= hours
                                ## else we set %rax to the remainder of %eax / 12
        cqto                    ## extend sign of rax to rdx
        movq $12, %r8           ## move 12 into the divisor register
      	movq $0, %rdx
      	idivq %r8               ## %rax / %r8 = quotient + rem
                                ## %rax == quotient, %rdx == rem
        movq %rdx, %rax         ## move the remainder into %rax
        movb $1, 6(%rsi)        ## set tod.ispm = 1
        jmp .NOT12
.AM:
        cmpq $12, %rax          ## compare 12 and hours
        jne .NOT12              ## if they arent equal, jump ahead
        movb $1, 6(%rsi)        ## if they are equal, set tod.ispm = 1

.NOT12:
        movw %ax, 0(%rsi)       ## move hours into tod.hours
        movq %rdi, %rax         ## move TIME_OF_DAY_SEC into %rax to prep for division
        cqto                    ## extend sign of eax to edx
        movq $3600, %r8         ## move 3600 into the divisor register
	      movq $0, %rdx
	      idivq %r8               ## %rax / %r8 = quotient + rem
                                ## %rax == quotient, %rdx == rem
        movq %rdx, %rax         ## now we do the same thing to the remainder
        cqto
        movq $60, %r8           ## we're dividing by 60 this time
	      movq $0, %rdx
	      idivq %r8               ## %rax / %r8 = quotient + rem
                                ## %rax == quotient, %rdx == rem
        movw %ax, 2(%rsi)      ## move quotient into tod.mins
        movw %dx, 4(%rsi)      ## move remainder into tod.secs
        cmpw $0, 0(%rsi)         ## compare 0 and tod.hours
        jne .RETURN             ## if they aren't equal, jump ahead
        movw $12, 0(%rsi)        ## set tod.hours = 12

.RETURN:
        movl $0, %eax           ## move 0 into the return value
        ret                     ## return

.error1:
        movl $1, %eax           ## move 1 into return value
        ret                     ## return 1 as an error value

### Data area associated with the next function
.data

other_int:                      # declare another accessible via name 'other_int'
        .int 0b1                # binary value as per C

rdi_extract:
        .int 0b1111111111111111

my_array:                ## declare multiple ints in a row
        .int	63         ## for an array. Each are spaced
        .int	6          ## 4 bytes from each other
        .int	91         ## issue, these are not ints/ (but not bigger than ints)
        .int	79
        .int	102
        .int	109
        .int	125
        .int	7
        .int	127
        .int	111
        .int	0

.text
.global  set_display_bits_from_tod

## ENTRY POINT FOR REQUIRED FUNCTION
## inputs
  ## int in register %edi
  ## pointer in register %rsi
set_display_bits_from_tod:
        ## assembly instructions here

## HOURS
        movw  rdi_extract(%rip), %dx
        movq  %rdi, %rax    ## move rdi into rax for safe keeping
        and   %ax, %dx      ## and ax into dx
        cmpw  $12, %dx
        jg    .ERROR2       ## if hours is larger than 12, error
        cmpw  $0, %dx
        jl    .ERROR2       ## if hours is less than zero, error

        movw  %dx, %ax      ## move tod.hours into ax for division
        cqto
        movw  $10, %cx
      	movw  $0, %dx
      	idivw %cx           ## %ax/ %cx == (quotient = %eax) + (rem = %edx)
      	movq  $0, %r10
      	movq  $0, %r11		  ## make sure the whole registers are 0 before moving
      	movw  %ax, %r10w    ## move quotient into hour_tens
        movw  %dx, %r11w    ## move remainder into hour_ones

## MINUTES

        movq  %rdi, %rax
        shrq   $16, %rax              ## shift by 16 to access minutes
        movw  rdi_extract(%rip), %dx  ## reset dx to all ones
        and   %ax, %dx
        cmpw  $59, %dx
        jg    .ERROR2                 ## if mins greater than 59, error
        cmpw  $0, %dx
        jl    .ERROR2                 ## if mins less than 0, error

        movw  %dx, %ax                ## move tod.mins into eax for division
        cqto
        movw  $10, %cx
      	movw  $0, %dx
      	idivw %cx                    ## %ax/ %cx == (quotient = %ax) + (rem = %dx)
      	movq  $0, %r8
      	movq  $0, %r9		             ## make sure the whole registers are 0 before moving
      	movw  %ax, %r8w              ## move quotient into min_tens
        movw  %dx, %r9w              ## move remainder into min_ones

## SECONDS

        movq  %rdi, %rax
        shrq   $32, %rax              ## shift by 32 to access seconds
        movw  rdi_extract(%rip), %dx  ## reset dx to all ones
        and   %ax, %dx
        cmpw  $59, %dx                ## if seconds greater than 59, error
        jg    .ERROR2
        cmpw  $0, %dx                 ## if seconds less than 0, error
        jl    .ERROR2
        movq  $0, %rdx                ## zero out the rdx register
        leaq  my_array(%rip),%rdx     ## load pointer to beginning of my_array into rdx

        movl  $0, (%rsi)              ## load 0s into (%rsi)

        movq  %rdi, %rax
        shrq   $48, %rax              ## shift by 48 to access ispm
        cmpb  $1, %al
        jne   .NOTPM                  ## if its not pm, jump to the else
        movl  other_int(%rip), %ecx   ## load 0b1 into mask register %ecx
        sal   $29, %ecx               ## shift mask to the left by 29
        or   %ecx, (%rsi)             ## or display and mask into display to set pm
        jmp  .JOIN                    ## unconditional jump so we avoid the else case

.NOTPM:
        movl  other_int(%rip), %ecx   ## load 0b1 into mask register %ecx
        sal   $28, %ecx               ## shift mask to the left by 28
        or   %ecx, (%rsi)             ## or display and mask into display to set am

.JOIN:
        cmpw  $0, %r10w
        jne   .SETBITS
        movw  $10, %r10w              ## if 0 = hours_tens, set it equal to 10 (for the empty mask)

.SETBITS:
        movl  (%rdx, %r10, 4), %ecx  ## use ecx as a temporary register for our mask
        sal   $21, %ecx              ## shift 21 to the left
        or   %ecx, (%rsi)            ## or mask into display

        movl  (%rdx, %r11, 4), %ecx  ## use ecx as a temporary register for our mask
        sal   $14, %ecx              ## shift 14 to the left
        or   %ecx, (%rsi)            ## or mask into display

        movl  (%rdx, %r8, 4), %ecx   ## use ecx as a temporary register for our mask
        sal   $7, %ecx               ## shift 7 to the left
        or   %ecx, (%rsi)            ## or mask into display

        movl  (%rdx, %r9, 4), %ecx   ## use ecx as a temporary register for our mask
        or   %ecx, (%rsi)            ## or mask into display

        movl  $0, %eax               ## move 0 into eax for success
        ret

.ERROR2:
        movl $1, %eax           ## move 1 into return value for error
        ret

.text
.global clock_update

clock_update:

      	pushq $0	                            ## for tod struct, %rsp now points to this
      	movq  %rsp, %rsi
      	movl	TIME_OF_DAY_SEC(%rip), %edi     ## copy global var to reg eax
      	call  set_tod_from_secs
      	cmpl  $1, %eax
      	je    .Exit                           ## if eax = 1, jump to return
      	movq  (%rsi), %rdi                    ## move tod into in %rdi
      	movl  $0, (%rsi)                      ## initialize display to zero
      	call  set_display_bits_from_tod
      	cmpl  $1, %eax
      	je    .Exit                           ## if eax = 1, jump to return
    	  movl	(%rsi), %ecx
      	movl  %ecx,CLOCK_DISPLAY_PORT(%rip)   ## copy display into global var
      	movl  $0, %eax
    	  jmp   .Exit

.Exit:
      	popq	%rcx                            ## reset stack and return
      	ret
