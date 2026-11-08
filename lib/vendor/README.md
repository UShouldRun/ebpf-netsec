# Vendored libraries

Copied in so the project builds on a fresh VM with no extra installs. Headers keep
the install layout, so code includes `<string8/string8.h>` and `<alloc/alloc.h>`
exactly as with a system install.

| Lib     | Source                                                             | Commit   | Kind        |
|---------|--------------------------------------------------------------------|----------|-------------|
| string8 | [UShouldRun/CString8](https://github.com/UShouldRun/CString8)       | d89c351+ | .c + header |
| alloc   | [UShouldRun/Arena-Allocator](https://github.com/UShouldRun/Arena-Allocator) | 18cb2bc  | .c + header |
| vector  | [UShouldRun/CVector](https://github.com/UShouldRun/CVector)         | a8c56eb  | header only |
| pair    | [UShouldRun/CPair](https://github.com/UShouldRun/CPair)             | afbdb07  | header only |

`+` = copied with changes not yet pushed upstream.

## Local patches

None: every copy matches its upstream commit except string8, whose header has
uncommitted changes.
