import re
from collections import defaultdict
from pathlib import Path

import flet as ft

CODE_TEXT_SIZE = 18
CODE_ROW_HEIGHT = 24

cannot_open_file_dlg = ft.AlertDialog(title=ft.Text("Cannot open the file"))

lexer_does_not_exist_dlg = ft.AlertDialog(
    title=ft.Text("Lexer file does not exist"),
    content=ft.Text("You should compile the program with --debug flag"),
)


def hover_scale(e: ft.ControlEvent):
    e.control.scale = 1.125 if e.control.scale == 1 else 1
    e.control.update()


def get_token_color(token_class: str) -> str:
    match token_class:
        case "Keyword":
            return ft.colors.TEAL_300
        case "Identifier":
            return ft.colors.DEEP_ORANGE_300
        case "Integer" | "Real" | "Boolean":
            return ft.colors.PURPLE_ACCENT_200
        case "Delimiter":
            return ft.colors.INDIGO_ACCENT_200
        case _:
            return ft.colors.LIGHT_BLUE_300


def parse_debug_line(line: str) -> tuple[str, str, int, int, int]:
    token_class_raw, _, token_type = re.split(r"\s*->\s*", line)
    token_class, args_raw = re.search(r"(.*)\{(.*)\}", token_class_raw).groups()

    line_number, start, end = re.search(r"Span\((\d*), (\d*), (\d*)\)", args_raw).groups()
    line_number, start, end = (
        int(line_number) - 1,
        int(start) - 1,
        int(end) - 1,
    )
    return token_class, token_type, line_number, start, end


def main(page: ft.Page):
    page.title = "Lexer Visualizer"
    page.vertical_alignment = ft.MainAxisAlignment.CENTER
    page.window.width = 1000

    def load_program(e: ft.FilePickerResultEvent):
        if not e.files:
            return

        if len(page.controls) >= 2:
            page.remove_at(1)
            page.update()

        if e.files[0].path is None:
            page.open(cannot_open_file_dlg)
            return
        olang_path = Path(e.files[0].path)

        lexer_path = olang_path.with_suffix(".lexer")
        if not lexer_path.is_file():
            page.open(lexer_does_not_exist_dlg)
            return

        line_indices = defaultdict(list)
        line_classes = defaultdict(list)
        with open(lexer_path, mode="r", encoding="utf-8") as fp:
            for line in fp:
                token_class, token_type, line_number, start, end = parse_debug_line(line.strip())

                line_indices[line_number].append((start, end))
                line_classes[line_number].append((token_class, token_type))

        with open(olang_path, mode="r", encoding="utf-8") as fp:
            lines_raw = fp.readlines()

            rows = []
            for i, line in enumerate(lines_raw):
                if not line.strip():
                    continue

                if i not in line_indices:
                    rows.append(
                        ft.Row(
                            [ft.Text(value=line, size=CODE_TEXT_SIZE)],
                            alignment=ft.MainAxisAlignment.START,
                            height=CODE_ROW_HEIGHT,
                        )
                    )
                    continue

                line_index = line_indices[i]
                line_class = line_classes[i]

                row = []
                if line_indices[i][0][0] != 0:
                    row.append(ft.Text(value=line[: line_index[0][0]], size=CODE_TEXT_SIZE))
                for (start, end), (token_class, token_type) in zip(line_index, line_class):
                    row.append(
                        ft.Container(
                            content=ft.Text(
                                value=line[start:end] + " ",
                                tooltip=ft.Tooltip(
                                    f"{token_class}: {token_type}",
                                    text_style=ft.TextStyle(size=CODE_TEXT_SIZE, color=ft.colors.BLACK),
                                ),
                                size=CODE_TEXT_SIZE,
                                color=get_token_color(token_class),
                            ),
                            on_hover=hover_scale,
                            scale=1,
                            animate_scale=300,
                        )
                    )
                if line_indices[i][-1][1] != len(line):
                    row.append(ft.Text(value=line[line_indices[i][-1][1]:], size=CODE_TEXT_SIZE))
                rows.append(ft.Row(row, alignment=ft.MainAxisAlignment.START, height=CODE_ROW_HEIGHT, spacing=0))

            page.add(
                ft.Container(
                    ft.Column(rows, expand=True, scroll=ft.ScrollMode.AUTO, spacing=0),
                    bgcolor=ft.colors.GREY_900,
                    padding=24,
                    expand=True,
                )
            )
            page.update()

    olang_file_picker = ft.FilePicker(on_result=load_program)
    page.overlay.append(olang_file_picker)
    page.update()

    page.add(
        ft.Row(
            [
                ft.ElevatedButton(
                    content=ft.Text(value="Choose .olang file...", size=18),
                    on_click=lambda _: olang_file_picker.pick_files(allowed_extensions=["olang"]),
                    height=40,
                ),
            ],
            alignment=ft.MainAxisAlignment.CENTER,
        ),
    )


ft.app(main)
