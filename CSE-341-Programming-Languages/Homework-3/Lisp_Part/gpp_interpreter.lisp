;(load "gpp_lexer.lisp")
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
(setq )
; interpreter help 
(defvar tokenStack (list))
(defvar valuefStack (list))
(defvar identifierStack (list))
(defvar removedList (list))

(defun split (str)
    (defvar wordList)
    (setq wordList '())
    (setq comment 0)
    (setq isExit 0)

    (loop for word in (split-str str " ") 
            do(progn
                    (setq word (cleanStr word))
                    (setq tempWord "")
                    (setq commentCount 0)
                    (setq flag 0)
                    (setq idx -1)
                    (loop for chr across word
                        do(progn    
                            (incf idx 1)
                            (setq flag 0)
                            ; check ( and )
                            (setq operatorLoc (inList chr OperatorList))
                            ; check if number may under zero
                            (if (and (= operatorLoc 1) (> (length word) 1) (= idx 0) (= commentCount 0))
                                (progn
                                    (if (string= chr "-")
                                        (progn
                                            (setq tempWord (concatenate 'string tempWord (list chr)))
                                            (setq flag 1)
                                        )
                                    )
                                )
                            )
                            (if (and (or (string-equal chr "(" ) (or (string-equal chr ")") (> operatorLoc -1 ))) (= flag 0))
                                (progn

                                    (if (and (not (string-equal tempWord "")))
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
                                            (setq flag 2)
                                            (push ";;" wordList)
                                            (setq commentCount 0)
                                            (setq comment 1)
                                        )
                                    )
                                )
                            )                    
                            ; add otherValues
                            (if (= flag 0)
                                (progn
                                    (if (= commentCount 1)
                                        (progn
                                            (setq isExit 1)
                                        )
                                    )
                                    (if (or (alpha-char-p chr)  (alphanumericp chr))
                                        (progn
                                            (setf tempWord (concatenate 'string tempWord (list chr)))
                                        )
                                        (progn
                                            (if (= comment 0)
                                                (progn
                                                    (setq isExit 1)
                                                )
                                            )  
                                        )
                                    )
                                )
                            )
                        )   
                    )
                    (if (and (= flag 0) (and (not (string-equal tempWord "")) (= isExit 0) ) )
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
            ;(format t "~a~%" (nth index KeywordTokenList))
            (setq tokenStack (appendList (nth index KeywordTokenList) tokenStack))
            (setq isUsed 1)
        )
    )
    ; if it is operator
    (setq index (inList word OperatorList) )
    (if
        (> index -1)
        (progn
            ;(format t "~a~%" (nth index OperatorTokenList))
            (setq tokenStack (appendList (nth index OperatorTokenList) tokenStack))
            (setq isUsed 1)
        )
    )
    ; if it is comment
    (if (equal word ";;")
        (progn
            ;(format t "Comment~%")
            (setq tokenStack (appendList "COMMENT" tokenStack))
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
    (setq isFloat 0)

    (loop for chr across word
        do (incf index 1)
        (setq temp (inList chr DigitList))
        (if (> temp -1)
            (progn
                (if (= index 0)
                    (setq firstCharDigit 1)
                )
                (if (> countB 0)
                    (progn
                        (if (not (string= chr "0"))
                            (setq isFloat 1)
                        )
                    )
                )
            )
        )
        (if (= index 0)
            (progn
                (if (string= chr "-")
                    (progn 
                        (setq index -1)  
                        (setq countOther (- countOther 1))  
                    )
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
                    ;(format t "VALUEF~%")
                    (if (= isFloat 0)
                        (progn
                            (setq isExit 1)
                            (setq isUsed 1)
                        )
                        (progn 
                            (setq tokenStack (appendList "VALUEF" tokenStack))
                            (setq valuefStack (appendList word valuefStack))

                            (setq isUsed 1)
                        )
                    )
                )
                (progn
                    ;(format t "SYNTAX_ERROR ~s cannot be tokenized~%" word)
                    (setq isUsed 1)
                    (setq isExit 1)
                    ;(return 0)
                )
            )
        )
        (progn
            (if (= isUsed 0)
                (progn 
                    ;(format t "IDENTEFIER~%")  
                    (setq tokenStack (appendList "IDENTIFIER" tokenStack))
                    (setq identifierStack (appendList word identifierStack))
                    (setq isUsed 1)
                )
            ) 
        )
    )
    ; syntax error
    (if (= isUsed 0)
        (progn 
            ;(format t "SYNTAX_ERROR ~s cannot be tokenized~%" word)
            (setq isExit 1)
        )
    )
)
(defun inList (word listToSearch) ; searches list if it founds return idx otherwise return -1
  (setq idx -1)
  (setq loc -1)
  (loop for w in listToSearch
    do (incf idx 1)    
    (if (string= word w)(setq loc idx))
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

        (if (string= word "exit")
            ;  finish program
            (do
                (setq isExit 1)
                (return 1)
            )
        )

        (if (not (string= word ""))
            (decideWord word)
        )
        ; control of is comment or is exit 
        (if  (= isComment 1) 
            (return 1)
        )

        (if (= isExit 1)
            (progn 
                (format t "SYNTAX_ERROR ~s cannot be tokenized~%" word)
                (exit 1)
            )
        )
    )
    (setq isComment 0)
)   
(defun appendList (item appendedList)

    (setq appendedList (append appendedList (list item)))
    appendedList
)
(defun getLineFromUser ()
    (getLine (read-line))
)
; end of load
(setf functionList (make-array '(20 20)))
(setf functionValueList (make-array '(20 20)))
(setf functionParametherNameList (make-array '(20 20)))

(setq functionCount 0)
(setq functionTokenCount 0)
(setq functionValueCount 0)
(setq opCount 0)
(setq ccCount 0)
(setq identifierCount 0)
(setq index 0)
(setq isFunction 0)
(setq valuefCount 0)
(setq functionParametherCount 0)

(defvar functionNameList (list))
(defvar functionCurrentValueList (list))
(defvar executionFunction (list))
(defvar parametherValues (list))

(defun gppinterpreter (&optional file-name)
    (if (not (equal file-name nil))
        (progn
            (getLinesFromFile file-name)
        )
        (progn
            (loop
                (getLineFromUser)
                (interpretLine)
            )
        )
    )
)
(defun interpretLine ()
    (setq opCount 0)
    (setq ccCount 0)
    (setq identifierCount 0)
    (setq index 0)
    (setq isFunction 0)
    (setq valuefCount 0)
    (setq functionParametherCount 0)
    (setq parametherDone 0)
    (setq isFunctionCall 0)

    (setq functionIndex -1)
    (setq valuefCountTemp 0)
    (setq indexTemp 0)
    (setq identifierCountTemp 0)
    (setq isFunctionDefined 0)

    (loop
        (setq token (nth index tokenStack))
        ; end of the loop
        (if (equal token nil)
            (progn
                (return)
            )
        )
        (if (= isFunction 1)
            (progn
                (if (= functionTokenCount 0)
                    
                    (setq functionNameList (appendList (nth identifierCount identifierStack) functionNameList)) ; get the function name and write it to the functionNameList 
                
                    (setq executionFunction (appendList token executionFunction))
                    
                )
                (setq functionTokenCount (+ functionTokenCount 1))
            )
        )
        (if (equal token "IDENTIFIER")
            (progn
                (if (= isFunction 0)
                    (progn

                        (setq functionIndex (inList (nth identifierCount identifierStack) functionNameList))
                        (if (> functionIndex -1)
                            (progn
                                (setq isFunctionCall 1)

                                (setq valuefCountTemp valuefCount)
                                (setq indexTemp index)
                                (setq identifierCountTemp identifierCount)
                            )
                        )

                        (setq identifierCount (+ identifierCount 1))
                    )
                    (progn
                        (if (= parametherDone 0)
                            (progn
                                (setf (aref functionParametherNameList functionCount functionParametherCount) (nth identifierCount identifierStack))   
                                (setq functionParametherCount (+ functionParametherCount 1))
                                (setq identifierCount (+ identifierCount 1))
                            )
                            (progn  
                                (setq functionCurrentValueList (appendList (nth identifierCount identifierStack) functionCurrentValueList))
                                ;; remove from valuef stack
                                (setq functionValueCount (+ functionValueCount 1))
                                (setq identifierCount (+ identifierCount 1))
                            )
                        )   
                    )
                )
            )
        )
        (if (equal token "KW_DEF")
            (progn
                (setq isFunctionDefined 1)
                (setq isFunction (+ isFunction 1))
            )
        )
        (if (equal token "OP_OP")
            (progn
                (setq opCount (+ opCount 1))
                (if (= isFunction 1)
                    (progn
                        (setq parametherDone 1)
                    )
                )
            )
        )
        (if (equal token "VALUEF")
            (progn
                (if (= isFunction 1)
                    (progn
                        (setq functionCurrentValueList (appendList (nth valuefCount valuefStack) functionCurrentValueList))
                        ;; remove from valuef stack
                        (setq valuefStack (remove-nth valuefCount valuefStack))
                        (setq functionValueCount (+ functionValueCount 1))
                    )
                    (progn
                        (setq valuefCount (+ valuefCount 1))
                    )
                )
            )
        )
        (if (equal token "OP_CP")
            (progn
                (setq ccCount (+ ccCount 1))
                (if (= isFunction 1)
                    (progn
                        (if (= opCount ccCount)
                            (progn
                                ;copy the execution function to the functionList[functionCount]
                                ;remove last element from executionFunction which is  OP_CP 
                                (setq executionFunction (remove-nth (- functionTokenCount 2) executionFunction))
                                (dotimes (j functionTokenCount )
                                    (setf (aref functionList functionCount j) (nth j executionFunction))
                                )
                                ;copy the execution function value to the functionValueList[functionCount]
                                (dotimes (j functionValueCount)
                                    (setf (aref functionValueList functionCount j) (nth j functionCurrentValueList))
                                )
                                ;function token count it holds also last close parenthesis of (def ..) so ignore it 
                                ;delete (def .. ... )
                                (setq functionTokenCount (+ functionTokenCount 1))
                                (dotimes (j functionTokenCount)
                                    (setq tokenStack (remove-nth (- index functionTokenCount) tokenStack))
                                )
                                (setq tokenStack (remove-nth 0 tokenStack))
                                (setf (aref functionParametherNameList functionCount functionParametherCount) (- functionParametherCount 1))   ; save last element as length of paramethers

                                (setq functionCount (+ functionCount 1))
                                (setq isFunction 0)
                                (setq functionTokenCount 0)
                                (setq executionFunction (list))
                                (setq functionCurrentValueList (list))

                                (setq index -1)
                                (setq opCount 0)
                                (setq ccCount 0)
                                (setq valuefCount 0)
                                (setq functionParametherCount 0)
                                (setq identifierCount 0)
                            )
                        )
                    )
                    (progn
                        (if (> opCount 0)
                            (progn
                                (if (= isFunctionCall 1)
                                    (progn
                                        (setq temp1 valuefCount)
                                        (setq temp2 index)
                                        (setq temp3 identifierCount)

                                        (setq valuefCount valuefCountTemp)
                                        (setq index indexTemp)
                                        (setq identifierCount identifierCountTemp)
                                        (decideParamethesisFunction functionIndex)

                                        ;(setq valuefCount temp1 )
                                        (setq index temp2 )
                                        (setq identifierCount temp3 )
                                        (setq controlIdx index)
                                        (loop 
                                            (if (equal (nth controlIdx tokenStack) "OP_CP")
                                                (progn
                                                    (return)
                                                )
                                                (progn
                                                    (setq controlIdx (+ controlIdx 1))
                                                )
                                            )
                                        )
                                        (setq index controlIdx)
                                        (executeNormalStatement)
                                    
                                        (setq identifierCount 0)
                                        (setq index -1)
                                        (setq opCount 0)
                                        (setq ccCount 0)
                                        (setq valuefCount 0)
                                        (setq isFunction 0)
                                        (setq isFunctionCall 0)
                                    )
                                    (progn
                                        (executeNormalStatement)
                                    )
                                )
                            )                       
                        )
                    )
                )
            )    
        )
        (setq index (+ index 1))
    )

    (if (= isFunctionDefined 1)
        (format t "#Function~%")
    
        (format t "Result : ~a~%" (nth 0 valuefStack))
    )

    (setq tokenStack (list))
    (setq valuefStack (list))
    (setq identifierStack (list))
)
(defun updateValuefCount ()
    (setq valuefCount 0)
    (setq i 0)
    (loop
        (if (= i index)
            (progn
                (return)
            )
        )
        (if (equal (nth i tokenStack) "VALUEF")
            (progn
                (setq valuefCount (+ valuefCount 1))
            )
        )
        
        (setq i (+ i 1))
    )
)
; execute the operation to the left of the OP_CP
; control of is function or not
; add here
; control of is there is an operation to the left of the OP_CP
(defun executeNormalStatement ()
    (updateValuefCount)
    (controlSyntaxParamethesis)
    (setq isUsed 1) 
    (setq valuef 0)
    ; check op_op op_plus valuef valuef op_cp
    (cond
        ( (equal (nth (- index 3) tokenStack) "OP_PLUS")                               
            (setq valuef (addValuef (nth (- valuefCount 2) valuefStack) (nth (- valuefCount 1) valuefStack)))
        )
        ( (equal (nth (- index 3) tokenStack) "OP_MINUS")                              
            (setq valuef (minusValuef (nth (- valuefCount 2) valuefStack) (nth (- valuefCount 1) valuefStack)))                             
        )
        ( (equal (nth (- index 3) tokenStack) "OP_MULT")                           
            (setq valuef (multValuef (nth (- valuefCount 2) valuefStack) (nth (- valuefCount 1) valuefStack)))                                
        )
        ( (equal (nth (- index 3) tokenStack) "OP_DIV")                  
            (setq valuef (divValuef (nth (- valuefCount 2) valuefStack) (nth (- valuefCount 1) valuefStack)))
        )
        (t (progn 
            (print "Syntax Error! Invalid User Input. Token/Parse is not valid.")
            (exit 0)
            )
        )
    )
    (if (equal isUsed 1)
        (progn
            (setf (nth (- index 4) tokenStack) "VALUEF")
            (setf (nth (- valuefCount 2) valuefStack) valuef)

            (setq valuefStack (remove-nth (- valuefCount 1) valuefStack))

            (setq tokenStack (remove-nth  (- index 3)  tokenStack))
            (setq tokenStack (remove-nth  (- index 3)  tokenStack))
            (setq tokenStack (remove-nth  (- index 3)  tokenStack))
            (setq tokenStack (remove-nth  (- index 3)  tokenStack))
            ;; her şeyi sıfırla ve başa dön
            (setq index -1)
            (setq opCount 0)
            (setq ccCount 0)
            (setq valuefCount 0)
            (setq isFunction 0)
        )
    )
)
(defun decideParamethesisFunction (functionLocation)
    ; decide how many parameters the function has
    (setq paramethesisCount 0)
    (setq i 1)
    (loop
        (if (equal (nth (+ i index) tokenStack) "OP_CP")
            (progn
                (return)
            )
            (progn
                (setq paramethesisCount (+ paramethesisCount 1))
            )
        )
        (setq i (+ i 1))
    )
    (setq i 0)
    ; get functionParametherNameList last index which is paramether count
    (loop
        (if (equal nil (aref functionParametherNameList functionLocation i) )
            (progn
                (return)
            )
        )
        (setq i (+ i 1))
    )

    (if (not (= (- i 2) paramethesisCount))
        (progn
            (format t "Syntax Error! Paramethesis Count Does Not Match~%")
            (exit 0)
        )
    )
    (executeFunctionStatement functionLocation paramethesisCount)
    (setq i 0)
    ; remove the values from tokenStack
    ; call executeFunctionStatement
)
(defun executeFunctionStatement (functionLocation parametherCount)
    ; def IDENTIFIER OP_OP VALUEF OP_CP
    ; token listesine func içeriğini koy
    ; valuefOfStack listesine funcDegerleriniKoy valuefCount degerini güncelle
    (setq fIndex 0)
    
    ;control of ( syntax error
    (controlSyntax (+ index -1) tokenStack "OP_OP")
    (setq tokenStack (remove-nth (+ index -1) tokenStack))  ; remove (
    (setq index (- index 1))

    (loop 
        (if (= fIndex parametherCount)
            (progn
                (return)
            )
        )
        ;possible if its function or not
        
        (setq parametherValues (appendList (nth valuefCount valuefStack) parametherValues))

        (setq tokenStack (remove-nth (+ index 1) tokenStack))   ; remove paramether
        (setq valuefStack (remove-nth valuefCount valuefStack))   ; remove valuef
        ;(setq identifierStack (remove-nth identifierCount identifierStack))   ; remove identifier

        (setq fIndex (+ fIndex 1))
    )
    (setq identifierStack (remove-nth identifierCount identifierStack))   ; remove identifier


    ;control of ) syntax error
    (controlSyntax (+ index 1) tokenStack "OP_CP")
    (setq tokenStack (remove-nth (+ index 1) tokenStack))   ; remove )

    (setq diffrence (aref functionParametherNameList functionLocation (+ 1 parametherCount)))
    (setq fIndex (+ 0 diffrence))

    (loop
        (setq token (aref functionList functionLocation fIndex))

        (if (equal token nil)
            (progn
                (return)
            )
        )
        (if (equal token "IDENTIFIER")
            (progn
                ; may come function or exp
                (setq token "VALUEF")
            )
        )

        (setq tokenStack (add-nth (+ index (- fIndex diffrence)) tokenStack token))
        
        (setq fIndex (+ fIndex 1))
    )

    ; remove the function name from the tokenStack
    (setq tokenStack (remove-nth (+ index (- fIndex diffrence)) tokenStack))

    ;(setq tokenStack (remove-nth (+ index fIndex) tokenStack))

    ; add valuef to the valuefStack
    (setq fIndex 0)
    (loop
        (setq valuef (aref functionValueList functionLocation fIndex))
        (if (equal valuef nil)
            (progn
                (return)
            )
        )
        (setq idx 1)
        (loop
            (if (= idx (+ parametherCount 1))
                (progn
                    (return)
                )
            )
            (if (equal valuef (aref functionParametherNameList functionLocation idx))
                (setq valuef (nth (- idx 1) parametherValues))
            )   

            (setq idx (+ idx 1))
        )

        (setq valuefStack (add-nth (+ valuefCount fIndex) valuefStack valuef))
        (setq fIndex (+ fIndex 1))
    )
    (setq parametherValues (list))
)
(defun addValuef (value1 value2)
    (setq value1List (getFlaotList value1))
    (setq value2List (getFlaotList value2))

    ; bölmeli toplama
    (if (not (= (nth 1 value1List) (nth 1 value2List)))
        (progn
            (setq den (* (nth 1 value1List) (nth 1 value2List)))
            (setq numer1 (* (nth 0 value1List) (nth 1 value2List)))
            (setq numer2 (* (nth 0 value2List) (nth 1 value1List)))

            (setq bolen (simplifyFlaot (+ numer1 numer2) den))

            (setq numer (write-to-string (/ (+ numer1 numer2) bolen)))
            (setq den (write-to-string (/ den bolen)))

            (setq result (concatenate 'string numer "b" den))
        )
        (progn
            (setq bolen (simplifyFlaot (+ (nth 0 value1List) (nth 0 value2List)) (nth 1 value1List)))
            
            (setq a (write-to-string (/ (+ (nth 0 value1List) (nth 0 value2List)) bolen) ))
            (setq b (write-to-string (/ (nth 1 value1List) bolen)))
            (setq result (concatenate 'string a "b" b))
        )
    )
    result
)
(defun minusValuef (value1 value2)
    (setq value1List (getFlaotList value1))
    (setq value2List (getFlaotList value2))
    ; özel durum value1SecondPart < value2SecondPart ise
    (if (not (= (nth 1 value1List) (nth 1 value2List)))
        (progn
            (setq den (* (nth 1 value1List) (nth 1 value2List)))
            (setq numer1 (* (nth 0 value1List) (nth 1 value2List)))
            (setq numer2 (* (nth 0 value2List) (nth 1 value1List)))

            (setq bolen (simplifyFlaot (- numer1 numer2) den))

            (setq numer (write-to-string (/ (- numer1 numer2) bolen)))
            (setq den (write-to-string (/ den bolen)))

            (setq result (concatenate 'string numer "b" den))
        )
        (progn
            (setq bolen (simplifyFlaot (- (nth 0 value1List) (nth 0 value2List)) (nth 1 value1List)))
            
            (setq a (write-to-string (/ (- (nth 0 value1List) (nth 0 value2List)) bolen) ))
            (setq b (write-to-string (/ (nth 1 value1List) bolen)))
            (setq result (concatenate 'string a "b" b))
        )
    )

    result
)
(defun multValuef (value1 value2)
    (setq value1List (getFlaotList value1))
    (setq value2List (getFlaotList value2))
    
    (setq bolen (simplifyFlaot (* (nth 0 value1List) (nth 0 value2List)) (* (nth 1 value1List) (nth 1 value2List))))

    (setq a (write-to-string ( / (* (nth 0 value1List) (nth 0 value2List)) bolen)))
    (setq b (write-to-string ( / (* (nth 1 value1List) (nth 1 value2List)) bolen)))

    (setq result (concatenate 'string a "b" b))

    result
)
(defun divValuef (value1 value2)
    (setq value1List (getFlaotList value1))
    (setq value2List (getFlaotList value2))
    (if (= 0 (nth 0 value2List))
        (progn
            (format t "Syntax Error! Divide By Zero~%")
            (exit 0)
        )
    )
    (setq bolen (simplifyFlaot (* (nth 0 value1List) (nth 1 value2List)) (* (nth 1 value1List) (nth 0 value2List))))
    
    (setq a (write-to-string (/ (* (nth 0 value1List) (nth 1 value2List)) bolen)))
    (setq b (write-to-string (/ (* (nth 1 value1List) (nth 0 value2List)) bolen)))

    (setq result (concatenate 'string a "b" b))

    result
)
(defun getFlaotList (value)
    (setq valueFirstPart (split-str value "b"))
    (setq valueSecondPart (parse-integer (nth 1 valueFirstPart)))
    (setq valueFirstPart (parse-integer (nth 0 valueFirstPart)))

    (list valueFirstPart valueSecondPart)
)
(defun remove-nth (indexL liste)
    (setq removedList (list))

    (setq i 0)
    (loop
        (if (equal i indexL)
            (progn
                (setq i (+ i 1))
                (continue)
            )
        )
        (if (equal (nth i liste) nil)
            (progn
                (return)
            )
        )
        (setq removedList (appendList (nth i liste) removedList))
        (setq i (+ i 1))
    )
    removedList
)
(defun add-nth (indexL liste value)
    (setq addedList (list))

    (setq i 0)
    (loop
        (if (equal i indexL)
            (progn
                (setq addedList (appendList value addedList))
            )
        )
        (if (equal (nth i liste) nil)
            (progn
                (return)
            )
        )
        (setq addedList (appendList (nth i liste) addedList))
        (setq i (+ i 1))
    )
    addedList
)
(defun controlSyntax (idx targetLoc target)
    (if (< idx 0)
        (progn
            (format t "Syntax Error! Invalid User Input~%")
            (exit 0)
        )
    )
    (if (not (equal (nth idx targetLoc) target))
        (progn
            (format t "Syntax Error! Does not entered PARANTEHESIS~%")
            (exit 0)
        )
    )
)
(defun controlSyntaxParamethesis()
    (setq i 0)
    (setq OPCount 0)
    (setq CCCount 0)
    (loop
        (if (equal (nth i tokenStack) "OP_OP")
            (progn
                (setq OPCount (+ OPCount 1))
            )
        )
        (if (equal (nth i tokenStack) "OP_CP")
            (progn
                (setq CCCount (+ CCCount 1))
            )
        )
        (if (equal (nth i tokenStack) nil)
            (progn
                (return)
            )
        )
        (setq i (+ i 1))
    )
    (if (not (= OPCount CCCount))
        (progn
            (format t "Syntax Error! Paramethesis Does Not Match~%")
            (exit 0)
        )
    )
)
(defun simplifyFlaot (numerator denominator)
    (setq i 2)
    (setq result 1)
    (loop
        (if (or (> i numerator) (> i denominator))
            (progn
                (return)
            )
        )
        (if (and (= (mod numerator i) 0) (= (mod denominator i) 0))
            (progn
                (setq numerator (/ numerator i))
                (setq denominator (/ denominator i))
                (setq result (* result i))
                (setq i 1)
            )
        )
        (setq i (+ i 1))
    )
    result
)
(defun getLinesFromFile (file-name)
    (let ((in (open file-name :if-does-not-exist nil)))
        (when in
            (loop for line = (read-line in nil)
                while line 
                    do (progn
                        (if (= isExit 0)
                            (progn 
                                (getLine line)
                                (interpretLine)
                            )
                        )
                    )
            )
            (close in)
        )
    )
)
(gppinterpreter)