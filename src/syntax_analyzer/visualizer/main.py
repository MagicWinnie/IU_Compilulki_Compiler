import sys
import pygraphviz as pgv
from treelib import Tree


IDENTATION = "  "  # 2 spaces


def main():
    if len(sys.argv) != 2:
        print(f"Usage: {sys.argv[0]} <PATH TO .syntax FILE>")
        exit(1)

    with open(sys.argv[1], mode="r") as fp:
        lines: list[str] = fp.readlines()
        lines = list(map(str.rstrip, lines))

    tree = Tree()

    parent_level = 0
    parent_node = [0]
    tree.create_node("Program", 0)

    for i in range(1, len(lines)):
        # Calculate indentation level
        level = (len(lines[i]) - len(lines[i].lstrip(IDENTATION))) // len(IDENTATION)

        # If the level is less than or equal to the current parent level, pop nodes until you find the correct parent
        while level <= parent_level:
            parent_node.pop()
            parent_level -= 1

        stripped_line = lines[i].lstrip()
        print(stripped_line, i, level, parent_level, parent_node)

        # Create a new node
        tree.create_node(stripped_line, i, parent=parent_node[-1])

        # If the level increased, update the parent node
        parent_level = level
        parent_node.append(i)

    # Export the tree to a dot file and render it as a PNG
    tree.to_graphviz(filename="tree.dot", shape="box", reverse=True)

    G = pgv.AGraph("tree.dot", rankdir="TB", strict=True)
    G.layout(prog="dot", args="")
    G.draw("tree.png")


if __name__ == "__main__":
    main()
