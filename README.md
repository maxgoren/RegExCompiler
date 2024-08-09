# RegExCompiler
regular expression pattern matching using NFA built by recursive descent compiler

Constructs NFA as transition table top down.

The following grammer for the 'standard' regular expression operations of concatenation, kleene closure, logical or, and parenthesized sub expressions is used by the parser
 to construct the NFA used to match strings.
 
      <expr> = <term>
      <term> = <factor> | <factor><term>
      <factor> = (<expr>) | <charachter> | <factor>*

Addtional operations such as wild card matching, the zero or one occurance closure, and at least one operator have been added.
 

 
