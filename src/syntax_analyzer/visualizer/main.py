import sys
import pygraphviz as pgv
from treelib import Tree

IDENTATION = "  "  # 2 spaces


def main():
    if len(sys.argv) != 3:
        print(f"Usage: {sys.argv[0]} <PATH TO .syntax FILE> <PATH TO OUTPUT FILE>")
        exit(1)

    with open(sys.argv[1], mode="r") as fp:
        lines: list[str] = fp.readlines()
        lines = list(map(str.rstrip, lines))

    tree = Tree()

    parent_level = 0
    parent_node = [0]
    tree.create_node("Program", 0)

    for i in range(1, len(lines)):
        level = (len(lines[i]) - len(lines[i].lstrip(IDENTATION))) // len(IDENTATION)

        while level <= parent_level:
            parent_node.pop()
            parent_level -= 1

        stripped_line = lines[i].lstrip()
        print(stripped_line, i, level, parent_level, parent_node)

        tree.create_node(stripped_line, i, parent=parent_node[-1])

        parent_level = level
        parent_node.append(i)

    tree.to_graphviz(filename="tree.dot", shape="box", reverse=True)

    G = pgv.AGraph("tree.dot", rankdir="TB", strict=True)
    G.layout(prog="dot", args="")
    G.draw(sys.argv[2])


if __name__ == "__main__":
    main()
