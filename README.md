# aGrep

Yet another `grep` clone — or at least, that's how it started. It turned out
to be less of a strict `grep` replica and more of a lightweight file
search / explorer: it can search for text inside files, search for files by
name, and do simple **fuzzy matching** so near-misses and typos still turn
up results.

## Features

- 🔍 **Text search** — search file contents for a given term, line by line.
- 📁 **File search** — search file/directory names across a directory tree.
- 🤏 **Fuzzy matching** — finds close matches even with typos, using
  Levenshtein-distance-based similarity, so you don't need to remember the
  exact spelling of what you're looking for.
- 🌲 **Recursive directory traversal** — searches subdirectories
  automatically, with an optional depth limit.
- 🙈 **Hidden file awareness** — skips dotfiles/dot-directories
  (`.git`, `.cache`, etc.) by default, with a flag to include them if
  needed.
- 🧱 **Binary file detection** — automatically skips binary files when
  searching text content.

## Why this exists

I built this project mainly to get more comfortable with C++ and sharpen my
fundamentals — that was the whole motivation, nothing more grandiose than
that. That said, it's turned out to be genuinely useful for my own
day-to-day use: when I forget which file a function or piece of logic lives
in, I can just point this at a directory and find it again.

## Dependencies

None, beyond:

- A C++ compiler with C++17 (or later) support
- The C++ standard library

No external libraries, no build system dependencies — just compile and run.

## Building

```bash
g++ -o agrep *.cpp
```

(Adjust the source file glob/paths to match your project layout.)

## Usage

```bash
./agrep [flags] [depth] [path] <target> [target...]
```

Flags can be combined into a single token (e.g. `-tvc`), and you can pass
**multiple search targets** in one run — every positional argument that
isn't consumed by a flag is treated as something to search for.

Some flags consume the *next* positional argument as their value (`-d` takes
a depth number, `-p` takes a path). Order matters for these: whatever
immediately follows the flag block is taken as that flag's value, so put
`-d <number>` and/or `-p <path>` right after your flags, before your search
terms.

### Flags

| Flag | Meaning | Takes a value? |
|------|---------|-----------------|
| `-f` | Search file/directory **names** | — |
| `-t` | Search file **contents** (text) | — |
| `-d` | Limit recursion depth | ✅ next argument = max depth (integer) |
| `-p` | Use an explicit path to search in | ✅ next argument = path |
| `-c` | Search starting from the current directory | — |
| `-g` | Global search (starting from your home directory) | — |
| `-h` | Include hidden files/directories (skipped by default) | — |
| `-v` | Verbose output (prints each file as it's searched) | — |

**Path resolution priority** (highest to lowest): `-p <path>` →
`-c` (current dir) → `-g` (home dir) → defaults to the current directory if
none are given.

### Examples

```bash
# Search file contents for "games" in the current directory
./agrep -tc games

# Verbose text search, limited to 2 levels deep, in an explicit path
./agrep -vdpt 2 ~/projects games

# Fuzzy file-name search across your whole home directory
./agrep -hgf .conifg

# Search file contents for multiple targets at once
./agrep -tc games config notes
```

## Status

This is a personal learning project and very much a work in progress.
Expect rough edges, and feel free to poke around the source if you're
curious how any of it works.

## License

No license has been chosen yet — treat this as "all rights reserved" for
now unless/until that changes.
