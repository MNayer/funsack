import click
from funsack.interface import iterate_functions

@click.command()
@click.option("--input", "-i", required=True, help="Input elf file.")
def funs(input):
    for function in iterate_functions(input):
        print(function)
