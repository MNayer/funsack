import click
import json
from funsack.interface import iterate_functions

@click.command()
@click.option("--input", "-i", type=click.Path(exists=True, file_okay=True,
    dir_okay=False, readable=True), required=True, help="Input elf file.")
@click.option("--output", "-o", type=click.Path(exists=False), default="", help="If set, the output is written to this file in .jsonl format")
def funs(input, output):
    if output != "":
        with open(output, "w") as f:
            for function in iterate_functions(input):
                f.write("%s\n" % json.dumps(function))
    else:
        for function in iterate_functions(input):
            print(function)
