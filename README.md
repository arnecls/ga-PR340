# ga-PR340

[https://ci.appveyor.com/api/projects/status/32r7s2skrgm9ubva?svg=true](https://ci.appveyor.com/api/projects/status/32r7s2skrgm9ubva?svg=true)


## Protocol

The server is expected to work on the following protocol:
`COMMAND [KEY] [VALUE]\n`

All fields can be put into double quotes `"` to allow spaces.
Double quotes inside a quoted string are escaped by a backslash `\`.

Responses are given as a newline `\n` terminated string.
The following error responses can be returned:

- `NOT FOUND` is returned by `GET` if the given KEY was not found.
- `OK` is returned by `SET` after a KEY VALUE pair has been written.
- `INVALID COMMAND` is returned if the given COMMAND is not known or could not be parsed.

The following commands are supported:

- `GET` returns a VALUE for a given KEY. Example `GET foo`
- `SET` stores a VALUE for a given KEY. Example `SET foo bar`
- `QUIT` closes the connection
