; G++ Lexical Syntax
(setq KeywordList (list "and" "or" "not" "equal" "less" "nill" "list" "append" "concat" "set" "def" "for" "if" "exit" "load" "display" "true" "false"))
(setq OperatorList (list "+" "-" "/" "*" "(" ")" ","))
(setq DigitList (list "1" "2" "3" "4" "5" "6" "7" "8" "9" "0"))
(setq IgnoredList (list '(#\Space #\Newline #\Backspace #\Tab #\Linefeed #\Page #\Return #\Rubout) ) )
(setq Comment ";;")

; G++ Lexer Token
(setq KeywordTokenList (list "KW_AND" "KW_OR" "KW_NOT" "KW_EQUAL" "KW_LESS" "KW_NIL" "KW_LIST" "KW_APPEND" "KW_CONCAT" "KW_SET" "KW_DEF" "KW_FOR" "KW_IF" "KW_EXIT" "KW_LOAD" "KW_DISPLAY" "KW_TRUE" "KW_FALSE"))
(setq OperatorTokenList (list "OP_PLUS" "OP_MINUS" "OP_DIV" "OP_MULT" "OP_OP" "OP_CP" "OP_COMMA"))

; exit comment control value
(setq isComment 0)
(setq isExit 0)
(setq readFromFileInput "f")

(defun gppStart()
    
    ; You can uncomment if you want to test your input / or cslip gpp_lexer.lisp < XXX.txt
    ;(format t "If you want to read from input.txt please write 'file' otherwise please enter anything... ~%")
    ;(setq readFromFileInput (read-line))
    ;(setq readFromFileInput (cleanStr readFromFileInput))

    ;(if (equal readFromFileInput "file")
    ;    (getLinesFromFile)
    ;)

    ; (format t "Write your codes of if you want to exit Type exit :~%") ; You have to uncomment to write your codes!
    
    (loop
       ; (format t "Enter your code Line : ~%") ; You have to uncomment to write your codes!
       (getLine (read-line)) ;calling readLine function to do operations
       (if (= isExit 1)
            (return 1)
       ) 

    )

)

(defun split (str)
    (defvar wordList)
    (setq wordList '())

    (loop for word in (split-str str " ") 
            do(progn
                    (setq tempWord "")
                    (setq commentCount 0)
                    (setq flag 0)

                    (loop for chr across word
                        do(progn    
                            (setq flag 0)
                            ; check ( and )
                            (if (or (string-equal chr "(" ) (string-equal chr ")"))
                                (progn
                                    (if (not (string-equal tempWord ""))
                                        (progn
                                            (push tempWord wordList)
                                            (setq tempWord "")
                                            (push chr wordList)
                                            (setq flag 1)
                                        )
                                        (progn 
                                          (setq flag 1)
                                          (push chr wordList)
                                        )
                                    )
                                )
                            )
                            
                            ; check ;;
                            (if (string-equal chr ";")
                                (progn
                                    (incf commentCount 1)
                                    (setq flag 1) ; special case
                                    (if (= commentCount 2)
                                        (progn
                                            (push tempWord wordList)
                                            (setq tempWord "")
                                            (setq flag 1)
                                            (push ";;" wordList)
                                            (setq commentCount 0)
                                        )
                                    )
                                )
                            )
                            
                            ; add otherValues
                            (if (= flag 0)
                                (progn
                                    (if (= commentCount 1)
                                        (progn
                                            (setf tempWord (concatenate 'string tempWord ";"))
                                            (setq commentCount 0)
                                        )
                                    )
                                    (setf tempWord (concatenate 'string tempWord (list chr)))
                                )
                            )
                        )   
                    )
                    (if (= flag 0)
                        (push tempWord wordList)
                    )   
            )
    )

    (reverse wordList)
)

(defun split-str (string &optional (separator " "))
  (split-str-1 string separator)
)

(defun split-str-1 (string &optional (separator " ") (r nil))
  (let ((n (position separator string
		     :from-end t
		     :test #'(lambda (x y)
			       (find y x :test #'string=)))))
    (if n
	(split-str-1 (subseq string 0 n) separator (cons (subseq string (1+ n)) r))
      (cons string r)))
)

(defun decideWord (word)
    (setq index -1)
    (setq isUsed 0)
    ; if it is keyword

    (setq index (inList word KeywordList) )

    (if
        (> index -1)
        (progn
            (format t "~a~%" (nth index KeywordTokenList))
            (setq isUsed 1)
        )
    )

    ; if it is operator
    (setq index (inList word OperatorList) )
    (if
        (> index -1)
        (progn
            (format t "~a~%" (nth index OperatorTokenList))
            (setq isUsed 1)
        )
        
    )
    
    ; if it is comment
    (if (equal word Comment)
        (progn
            (format t "Comment~%")
            (setq isComment 1)
            (setq isUsed 1)
        )
    )
    ; if it is identifier
    ; if it is fraction
    (setq countB 0)
    (setq countOther 0)
    (setq firstCharDigit 0)
    (setq index -1)
    (setq temp -1)

    (loop for chr across word
       
        do (incf index 1)
 
        (setq temp (inList chr DigitList))
        (if (> temp -1)
            (progn
                (if (= index 0)
                    (setq firstCharDigit 1)
                )
            )
        )
        (if (string= chr "b")
            (progn(incf countB 1))
            (progn 
                (if(not (> temp -1))
                    (incf countOther 1))
            )
        )
    )


    (if (= firstCharDigit 1)
        (progn
            (if (and (= countB 1) (= countOther 0))
                (progn
                    (format t "VALUEF~%")
                    (setq isUsed 1)
                    ;(return 0)
                )
                (progn
                    (format t "SYNTAX_ERROR ~s cannot be tokenized~%" word)
                    (setq isUsed 1)
                    (setq isExit 1)
                    ;(return 0)
                )
            )
        )
        (progn
            (if (= isUsed 0)
                (progn (format t "IDENTEFIER~%")  
                    (setq isUsed 1)
                )
            ) 
        )
    )
    

    ; syntax error
    (if (= isUsed 0)
        (progn 
            (format t "SYNTAX_ERROR ~s cannot be tokenized~%" word)
            (setq isExit 1)
        )
    )

)

(defun inList (word listToSearch) ; searches list if it founds return idx otherwise return -1
  (setq idx -1)
  (setq loc -1)
  (loop for w in listToSearch
  
    do (incf idx 1)
        
    (if (string= word w)
        (setq loc idx)
    )
    
  )

  loc
)

(defun cleanStr (str)
    (string-trim '(#\Space #\Newline #\Backspace #\Tab #\Linefeed #\Page #\Return #\Rubout) str)
)

(defun getLine (line) ; user input line
    ; we have to write line as word like => for word in line.split(" ")
    (setq line (cleanStr line))

    (loop for word in (split line)
        do
        ; remove Ignored characters from word
        (setq word (cleanStr word))
        (setq isComment 0)
        (if (string= word "exit")
            ;  finish program
            (do
                (setq isExit 1)
                (return 1)
            )
        )

        (decideWord word)

        (if (or (= isComment 1) (= isExit 1))
            (return 1)
        )

    )
)   

(defun getLinesFromFile()

    (let ((in (open "input.txt" :if-does-not-exist nil)))
        (when in
            (loop for line = (read-line in nil)
            
                while line 
                    do (progn
                        (if (= isExit 0)
                            (getLine line)
                        )
                    )
            )
            (close in)
        
        )
    )

)

(gppStart)
