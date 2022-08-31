import os
import random
import discord
import glob
import requests
from discord.ext import commands
from discord.ext import tasks
from dotenv import load_dotenv

images = []
for filename in glob.glob('pics/*.jpg'):
    images.append(filename)

load_dotenv()
TOKEN = os.getenv('DISCORD_TOKEN')
GUILD = os.getenv('DISCORD_GUILD')

intents = discord.Intents.all()
intents.members = True

help_command = commands.DefaultHelpCommand(
    no_category = 'Commands'
)

bot = commands.Bot(command_prefix = '!', intents = intents, help_command = help_command)

lastEntryDate = None

def checkWaterLevel():
    msg = requests.get("")
    date = msg.json()['feeds'][-1]['created_at']
    msg = msg.json()['feeds'][-1]['field1']

    return int(msg), date

@bot.command(help = "Woof woof istiyosan woofla!")
async def woof(ctx):
    response = "Woof woof!"
    await ctx.send(response)

@bot.command(help = "Su seviyesi için su woof woofla!")
async def water(ctx):
    response = "Su seviyesi için su woof woofla!"
    await ctx.send(response)

@bot.command(help = "Susma foto için susmaVre!")
async def susmaVre(ctx):
    response = discord.File(images[random.randrange(0, len(images))])
    await ctx.send(file = response)
    response = response.filename
    await ctx.send(response.split('.', 1)[0])

@bot.event
async def on_ready():
    await checkWaterLevelTask.start()
    await bot.change_presence(activity = discord.Activity(type = discord.ActivityType.playing, name = "Woof woof!"))

@bot.event
async def on_message(message):
    if message.author == bot.user:
        return

    if message.content == "Su" or message.content == "su" or message.content == "SU":
        response = format(message.author.mention) + ", su seviyesi " + str(checkWaterLevel()[0])
        await message.channel.send(response)

    if message.content == "Woof" or message.content == "woof" or message.content == "havla" or message.content == "Havla":
        response = "Woof woof!"
        await message.channel.send(response)

    await bot.process_commands(message)

@tasks.loop(minutes = 10)
async def checkWaterLevelTask():
    global lastEntryDate
    if checkWaterLevel()[0] < 300 and checkWaterLevel()[1] != lastEntryDate:
        lastEntryDate = checkWaterLevel()[1]
        channel = bot.get_channel()
        await channel.send("Woof susadım woof!")

if __name__ == '__main__':
    bot.run(TOKEN)