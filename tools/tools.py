import click
from funsack import collect_subprogram_dies, dies

@click.command()
@click.option("--input", "-i", required=True, help="Input elf file.")
def funs(input):
    collect_subprogram_dies(input.encode("ascii"))
    for die in dies:
        print(die)
