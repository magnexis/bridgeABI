# BridgeABI interface grammar (Phase 2 subset)

```text
document       := "namespace" qualified_name ";" attribute? interface
attribute      := "@id" "(" string ")"
interface      := "interface" identifier "version" integer "." integer "{" member* "}"
member         := capability | threading | method
capability     := "capability" string "version" integer "." integer ";"
threading      := "threading" identifier ";"
method         := ownership? type identifier "(" parameters? ")" ";"
parameters     := parameter ("," parameter)*
parameter      := ownership? type nullable? identifier
ownership      := "borrowed" | "owned" | "transferred" | "shared" | "static" | "host_owned" | "plugin_owned"
nullable       := "?"
type           := identifier
```

This intentionally restricted grammar is whitespace-insensitive and does not parse C++. `string` and `buffer` parameters and return values require an explicit ownership modifier. An interface must declare an `@id` and a non-zero major version.
