import os
import time
import random

os.system('cls')

#Declarando variáveis e vetores
shopInv = ['Elmos', 'Cota', 'Luvas', 'Botas', 'Espadas', 'Escudos']
shopPrices = [20, 80, 30, 40, 35, 50]
playerInv = [0, 1, 0, 1, 1, 0]
isDone = False

#Definir funcoes
def printShop(shopInv, shopPrices):
    for i in range(len(shopInv)):
        print(i+1, '-', shopInv[i], ' --', shopPrices[i], ' moedas')

def printInv(playerInv, shopInv, playerCoins):
    print('\n *** Seu Inventário ***')
    for i in range(len(playerInv)):
        if(playerInv[i] > 0):
            print(shopInv[i], ' x ', playerInv[i])
    print('Você tem ', playerCoins, ' moedas')

def checkOp():
    op = input('Deseja comprar(C), vender(V), sair(S) ou roubar a loja(R)?\n')
    while (op != 'c' and op != 'C' and op != 'v' and op != 'V' and op != 's' and op != 'S' and op != 'r' and op != 'R'):
        print('Opção inválida!')
        op = input('Digite outra opção: ')
    return op

def comprar():
    global playerCoins
    escolha = int(input('Digite o numero do item que deseja comprar:  '))
    if(1 <= escolha <= 6):
        quantidade = int(input('Quantos você quer comprar?'))
        chanceDefeito = random.randint(0, 100)
        # Ver se o jogador tem moedas o suficiente pra comprar os itens
        # Aqui o jogador tem moedas o suficiente
        if(playerCoins >= shopPrices[escolha-1]*quantidade):
            #Alterar abaixo para mudar a chance de um produto vir com defeito
            # Se for menor que o numero, um produto virá com defeito
            #Produto veio com defeito
            if(chanceDefeito > 10):
                playerInv[escolha-1] += quantidade
            #Produto n veio com defeito
            else:
                playerInv[escolha-1] += quantidade - 1
                op = str(input('Um dos itens comprados veio com defeito! Gostaria de tentar convencer o vendedor a trocar? (Rolar D20)\nS/N'))
                #Tentar trocar o item
                if(op == 's' or op == 'S'):
                    tentarConvencer = random.randint(1,20)
                    #Chance de convencimento
                    #Alterar abaixo para dar mais ou menos defeito (1 - 20) DEFAULT: 12
                    if(tentarConvencer >= 12):
                        print('Você conseguiu convencer o vendedor a trocar o produto :D')
                        playerInv[escolha-1] += 1
                        time.sleep(1.5)
                    else:
                        print('Você não conseguiu convencer o vendedor D:')
                        time.sleep(1.5)

            playerCoins -= shopPrices[escolha-1]*quantidade

        #O jogador n tem moedas o suficiente
        else:
            print('Você não tem moedas o suficiente')
            time.sleep(1.5)
    else:
        print('Opção inválida!')
        time.sleep(1.5)

def vender():
    global playerCoins, playerInv, shopPrices
    escolha = int(input('Digite o numero do item que deseja vender: '))
    if(1 <= escolha <= 6):
        quantidade = int(input('Quantos você deseja vender?'))
        if(playerInv[escolha - 1] >= quantidade):
            playerInv[escolha - 1] -= quantidade
            playerCoins += shopPrices[escolha - 1] * quantidade
        else:
            print('Você não tem isso tudo!!')
            time.sleep(1.5)
    else:
        print('Opção inválida!!')
        time.sleep(1.5)

def sair():
    global isDone
    isDone = True
    print('\nAté mais!')

def roubar():
    global isDone, playerInv
    escolha = int(input('Digite o numero do item que deseja roubar (20% de chance): '))
    if(1 <= escolha <= 6):
        chance = random.randint(0, 100)
        #Chance de roubar a loja. Se for maior ou igual a 80, o player consegue roubá-la
        if(chance >= 80):
            playerInv[escolha-1] += 1
            print('Você conseguiu roubar o item :D')
            time.sleep(1.5)
        else:
            print('Ladrão! Você foi preso!')
            isDone = True
            time.sleep(1.5)
    else:
        print('Opção inválida!')
        time.sleep(1.5)

# --------------------------------------------------------------------------------------------------------------

#Game Loop
print('Olá aventureiro! Seja bem-vindo à minha loja!\n')
playerCoins = int(input('Quantas moedas você tem?\n'))

while(isDone == False):
    os.system('cls')
    printShop(shopInv, shopPrices)
    printInv(playerInv, shopInv, playerCoins)
    escolha = checkOp()

    if(escolha =='c'or escolha =='C'):
        comprar()

    elif(escolha == 'v' or escolha == 'V'):
        vender()

    elif(escolha == 's' or escolha == 'S'):
        sair()

    elif(escolha == 'r' or escolha == 'R'):
        roubar()

os.system("PAUSE")
