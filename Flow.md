character -> any ASCII character
digit -> any digit from 0 to 9

v_character -> any VISIBLE `character`
i_character -> any INVISIBLE `character`
n_character -> any `character` that represents to a `digit`
e_character -> any escape `character`: %, \

Space -> any consecutive set of `i_character`s

Word -> any consecutive set of `v_character`s
String -> any consecutive set of `character`s surrounded by a set of "

Number -> any consecutive set of `digit`s that doesn't have any 0s in the beginning

Value -> any `Word`, `String` or `Number`


Type:
the `Word` int
the `Word` char
the `Word` bool
the `Word` float
the `Word` double
the `Word` short
the `Word` void

the `Word` long, followed by the `Word` int
the `Word` long, followed by the `Word` double

the `Word` long, followed by the `Word` long, followed by the `Word` int

the `Word` unsigned, followed by any of the `Word`s or sets of `Word`s from before
the `Word` signed, followed by any of the `Word`s or sets of `Word`s from before

Variable Declaration:
any `Type`, optionally followed by a `character` *, followed by any `Word`, that doesn't begin with a `n_character`, followed by either a `character` , or `character` ;
any `Word`, that doesn't begin with a `n_character`, optionally precedented by a `character` *, put after a `character` , following a `Variable` defined before

Function Call:
any `Word`, that doesn't begin with a `n_character`, followed by a `character` (
  followed by a set of 

Function Declaration:
any `Type`, optionally followed by a `character` *, followed by any `Word`, that doesn't begin with a `n_character`, followed by a `character` (
  followed by either a set of `Type`s, each `Type`, followed by a `character` , or a set of `Variable`s, without the `character` ; when the set ends
  followed by a `character` ), followed by a `character` ;

Preprocessor:
the `character` # followed by any of these `Word`s: include, define, undef, ifdef, ifndef
when using the include `Word`: follow with, optionally the `character` <, any `String`, followed again by the `character` >, if the `character` < was used before
when using the define `Word`: follow with any `Word`, optionally followed by any `Value`
when using the undef `Word`: follow with any `Word`
when using the ifdef `Word`: follow with any `Word`
when using the ifndef `Word`: follow with any `Word`
