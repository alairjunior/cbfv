# cbfv

There's probably a ton of circular buffer implementations in C++ available. So, why creating another one? The goal of this repository is not providing another circular buffer implementation, but to show how to use formal verification for real.

----

## Requirements

To perform a formal verification of this system, we recommend using [cbmc](https://www.cprover.org/cbmc/). We recommend downloading version 5.11 or later. Be careful when installing `cbmc` using the package manager of your distribution as it may be an old version that does not support some constructions used in the Circular Buffer code. 

**cbmc** will not test your code, in the sense that it will not generate test cases and check if the code behave properly at runtime. **cbmc** will create a formal representation of the code and prove that the code satisfies the set of provided assertions. So, if the code passes the tests you can rest assured that the assertions will hold for **every** input you provide, considering the assumptions you defined during the verification. 

## Source Description

There are two files in this repository:

  1. `circular_buffer.h`, which provides the actual implementation of the circular buffer
  2. `verify.cpp`, which is the code holding the assertions that are going to be verified using cbmc.

We are defining 6 assertions in `verify.cpp`. To completely check the circular buffer, it would be necessary some more. However, this is just an example and you feel free to define others and inform us of your findings. For all assertions, we assume the capacity of the circular buffer to be greater than 0.

  1. Assuming that we have *N* elements in the circular buffer and *N* is bellow the circular buffer capacity, if we push another element, the size of the circular buffer must increase by one;
  2. Assuming that we have *N* elements in the circular buffer and *N* is higher or equal than the circular buffer capacity, if we push another element, the size of the circular buffer must remain the same;
  3. Assuming that we have *N* elements in the circular buffer and *N* is greater than 0, if we pop one element from the circular buffer, its size must decrease by one;
  4. Assuming that we have *N* elements in the circular buffer and *N* is greater than 0 and the circular buffer's capacity is greater or equal to *N*, the popped element must be equal to the first element pushed into the circular buffer;
  5. Assuming that we have *N* elements in the circular buffer and *N* is greater than 0 and the circular buffer's capacity is lower than *N*, the popped element must be equal to (*N* - capacity)th element pushed into the circular buffer.


## Usage

After downloading cbmc, you can verify the code using the following command:

```
cbmc verify.cpp --function testAll --unwind 12 -D CBMC_CHECKING
```

### Explanation

  * `--function testAll` defines that the function to be tested is called `testAll`. This function calls all other functions holding the assertions to be checked;
  * `--unwind 12` states that we are only checking until depth 12. This means that we are checking circular buffers of up to 12 elements. Increasing this number impacts the time required to perform the verification;
  * `-D CMBC_CHECKING` defines the preprocessor macro `CBMC_CHECKING`. We are using this macro in the source code of `circular_buffer.h` to remove the exception handling. **cbmc**, as of now, complains about some types used for exception handling.

### Suggestions

To understand better how things work, you can change the assertions so that they are wrong. Do some minor changes and execute **cbmc** to see what happens. One big advantage of formal verification is the ability of providing a counterexample if an assertion fail. You can ask **cbmc** to provide a counterexample by adding `--trace' to the command line. 

Also, you can define assertions to verify other properties of the circular buffer. The method peek, for example, is not covered by the assertions provided in `verify.cpp`

