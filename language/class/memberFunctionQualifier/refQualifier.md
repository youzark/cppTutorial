# Member function ref-qualifier
[source](https://stackoverflow.com/questions/21861148/what-does-the-single-ampersand-after-the-parameter-list-of-a-member-function-dec) 

A function without a ref-qualifier can be called for both rvalue and lvalues. A function with a && ref-qualifier can only be called for rvalues. A function with a & ref-qualifier can only be called for lvalues.

```cpp
class No { void foo(); };
class L { void foo() &; };
class R { void foo() &&; };

No().foo(); // valid
No no; no.foo(); // valid
L().foo(); // invalid
L l; l.foo(); // valid
R().foo(); // valid
R r; r.foo(); // invalid

```
