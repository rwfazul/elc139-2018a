/* 
    $ chpl -o hello_world hello_world.chpl --fast
    $ ./chpl --message="Um novo Hello World!"
*/

module Hello {

  config const message = "Hello, world!";
  
  proc main() {
    writeln(message);
  }
  
}
