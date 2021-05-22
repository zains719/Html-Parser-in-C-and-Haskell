import System.IO
import Data.Char

-- TAG TOKENS
html_ot = 1
html_ct = 2
head_ot = 3
head_ct = 4
title_ot = 5
title_ct = 6
body_ot = 7
body_ct = 8
div_ot = 9
div_ct = 10
p_ot = 11
p_ct = 12
h1_ot = 13
h1_ct = 14
h2_ot = 15
h2_ct = 16
h3_ot = 17
h3_ct = 18
a_ot = 19
a_ct = 20
ul_ot = 21
ul_ct = 22
li_ot = 23
li_ct = 24
br = 25
hr = 26
text = 27

main = do  
        handle <- openFile "file.html" ReadMode
        contents <- hGetContents handle
        let tagTokens = lexer contents "" False False
        if (parser tagTokens [] 0 [0, 0, 0, 0] False False) then print "PARSING SUCCESSFULL: 'file.html' is a valid HTML file" else errorDetected
        hClose handle

errorDetected = error ("PARSING FAILED: 'file.html' is not a valid HTML file")

-- LEXING
lexer :: String -> String -> Bool -> Bool -> [Int]
lexer [] _ _ _ = []
lexer (x:xs) currentTag inTag tagReading 
    | inTag = 
        case () of 
            () | x == '<' -> errorDetected
               | x == '>' -> (tokeniseTag (currentTag ++ [x])) : lexer xs "" False False
               | xs == [] -> errorDetected 
               | not tagReading -> lexer xs currentTag inTag tagReading
               | isWhiteSpace x -> lexer xs currentTag inTag False
               | otherwise -> lexer xs (currentTag ++ [x]) inTag tagReading
    | otherwise = 
        case () of
            () | x == '>' -> errorDetected
               | x == '<' -> lexer xs (currentTag ++ [x]) True True
               | not (isWhiteSpace x) -> (tokeniseTag [x]) : lexer xs currentTag inTag tagReading
               | otherwise -> lexer xs currentTag inTag tagReading


-- PARSING
parser :: [Int] -> [Int] -> Int -> [Int] -> Bool -> Bool -> Bool
parser [token] stack _ tagFreq headJustSeen _
    | (token /= html_ct || length stack > 1 || not (isTagFreqValid tagFreq) || headJustSeen) = False
    | otherwise = True
parser (token:tokens) stack counter tagFreq headJustSeen inP
    | (counter == 0 && token /= html_ot) = errorDetected
    | headJustSeen = 
        case () of 
            () | token == body_ot -> parser tokens (operateOnStack stack token inP) (counter+1) (updateTagFreq tagFreq token) False inP
               | token == text -> parser tokens stack (counter+1) tagFreq headJustSeen inP
               | otherwise -> errorDetected
    | isOneFreq token = parser tokens (operateOnStack stack token inP) (counter+1) (updateTagFreq tagFreq token) headJustSeen inP
    | token == head_ct = parser tokens (operateOnStack stack token inP) (counter+1) tagFreq True inP
    | token == p_ot = parser tokens (operateOnStack stack token inP) (counter+1) tagFreq headJustSeen True
    | token == p_ct = parser tokens (operateOnStack stack token inP) (counter+1) tagFreq headJustSeen False
    | (token == br || token == hr || token == text) = parser tokens stack (counter+1) tagFreq headJustSeen inP
    | otherwise = parser tokens (operateOnStack stack token inP) (counter+1) tagFreq headJustSeen inP


-- LEXING AUX FUNCTIONS    

tokeniseTag :: String -> Int
tokeniseTag cs 
    | cs == "<html>" = html_ot | cs == "</html>" = html_ct | cs == "<head>" = head_ot | cs == "</head>" = head_ct
    | cs == "<title>" = title_ot | cs == "</title>" = title_ct | cs == "<body>" = body_ot | cs == "</body>" = body_ct
    | cs == "<div>" = div_ot | cs == "</div>" = div_ct | cs == "<p>" = p_ot | cs == "</p>" = p_ct
    | cs == "<h1>" = h1_ot | cs == "</h1>" = h1_ct | cs == "<h2>" = h2_ot | cs == "</h2>" = h2_ct
    | cs == "<h3>" = h3_ot | cs == "</h3>" = h3_ct | cs == "<a>" = a_ot | cs == "</a>" = a_ct
    | cs == "<ul>" = ul_ot | cs == "</ul>" = ul_ct | cs == "<li>" = li_ot | cs == "</li>" = li_ct
    | cs == "<br>" = br | cs == "<hr>" = hr
    | length cs == 1 = text
    | otherwise = errorDetected

isWhiteSpace :: Char -> Bool
isWhiteSpace c 
    | (c == ' ' || c == '\n' || c == '\t') = True
    | otherwise = False


-- PARSING AUX FUNCTIONS

isOneFreq :: Int -> Bool
isOneFreq token
    | (token == html_ot || token == head_ot || token == title_ot || token == body_ot) = True
    | otherwise = False

updateTagFreq :: [Int] -> Int -> [Int]
updateTagFreq [html, head, title, body] token
    | ((html > 0 && token == html_ot) || (head > 0 && token == head_ot) || (title > 0 && token == title_ot) || (body > 0 && token == body_ot)) = errorDetected
    | otherwise =
        case () of
            () | token == html_ot -> [html + 1, head, title, body]
               | token == head_ot -> [html, head + 1, title, body]
               | token == title_ot -> [html, head, title + 1, body]
               | token == body_ot -> [html, head, title, body + 1]

isTagFreqValid :: [Int] -> Bool
isTagFreqValid [html, head, title, body] 
    | (html == 0 || head == 0 || body == 0) = False
    | otherwise = True

operateOnStack :: [Int] -> Int -> Bool -> [Int]
operateOnStack [] token inP
    | token `mod` 2 /= 0 = token : []
    | otherwise = errorDetected
operateOnStack (s:st) token inP
    | token `mod` 2 /= 0 = 
        case () of 
            () | isNestingValid s token inP -> token:(s:st)
               | otherwise -> errorDetected
    | otherwise =
        case () of 
            () | s == (token - 1) -> st
               | otherwise -> errorDetected

isNestingValid :: Int -> Int -> Bool -> Bool
isNestingValid s token inP
    | (s == html_ot && token /= head_ot && token /= body_ot) || (s == title_ot) || 
      (s == head_ot && token /= title_ot) || (token == title_ot && s /= head_ot) ||
      (inP && (token == p_ot || token == div_ot)) = False
    | otherwise = True





    