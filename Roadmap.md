# QuickNet Roadmap

[X] Phase 1:

    [X] first design:
        [X] MVC or flask-style?
        [X] how to assign addresses for functions?
        
    [X] write the prototype, definitions of functions-classes without implementation

[ ] Phase 2:
    
    [ ] Socket classes
        [X] make copy constructors
        [ ] be able to reset port (use it again)     
        [ ] testing:
            [ ] accept and listen work properly 
            [ ] receive works within timeout, and detects it

    [ ] http Request
        [ ] testing:
            [ ] parses 
                [ ] big packages
                [ ] minimum package
                [ ] no headers
                [ ] different messages
                [ ] no messages
            [ ] doesnt parse
                [ ] invalid ending of lines (no crlf)
                [ ] many crlf at end of line
                [ ] invalid headers
                
    [ ] http Response:
        [ ] contains only necessary headers
        [ ] same testing of http Request