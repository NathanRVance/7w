################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ai.c \
../ai_trade.c \
../ai_weights.c \
../ai_weights_special.c \
../cards.c \
../color.c \
../data.c \
../decks.c \
../endgame.c \
../history.c \
../io.c \
../log.c \
../main.c \
../messenger.c \
../player_turn.c \
../pregame.c \
../science.c \
../special_actions.c \
../special_cards.c \
../trade.c \
../util.c \
../view.c \
../war.c \
../wonder.c 

OBJS += \
./ai.o \
./ai_trade.o \
./ai_weights.o \
./ai_weights_special.o \
./cards.o \
./color.o \
./data.o \
./decks.o \
./endgame.o \
./history.o \
./io.o \
./log.o \
./main.o \
./messenger.o \
./player_turn.o \
./pregame.o \
./science.o \
./special_actions.o \
./special_cards.o \
./trade.o \
./util.o \
./view.o \
./war.o \
./wonder.o 

C_DEPS += \
./ai.d \
./ai_trade.d \
./ai_weights.d \
./ai_weights_special.d \
./cards.d \
./color.d \
./data.d \
./decks.d \
./endgame.d \
./history.d \
./io.d \
./log.d \
./main.d \
./messenger.d \
./player_turn.d \
./pregame.d \
./science.d \
./special_actions.d \
./special_cards.d \
./trade.d \
./util.d \
./view.d \
./war.d \
./wonder.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


