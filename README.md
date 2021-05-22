# Html-Parser-in-C-and-Haskell
Implement the two versions of the program for as much of the specification listed below that you
have time to complete. You should use the standard C and Haskell tools and default libraries that
you used when taking the module during term 1. You should not add any third-party libraries, or
write code that will not work with the standard tools. For Haskell, see chapter 9 of “Learn You a
Haskell” if you need some help with reading from a file.
The amount of code required for each program should be relatively small, sufficient to produce a
working answer. The code should be well-designed and written.
Each program should read its input from a file containing the HTML document to check, in plain
text format (i.e., created using a normal editor such as Visual Studio Code), and:

• Confirm that each tag is a valid HTML tag (see below for which tags).
• Check that each tag has a corresponding closing tag if needed.
• Check that the tags are nested properly.
• Check that there are <html> and </html> tags around the entire document.
• Check that there is a single <head>…</head> section followed by a single <body>…</body> section in that order.

If an error is found an error message should be displayed and the program stops, so only the first
error found is reported. If no errors are found the program should display a simple message that
the HTML was correct.

Constraints:
• You must work with the following subset of HTML tags only, not every tag! This mandatory subset is {html, head, body, title, h1, h2, h3, p, ul, li, a, div, br, hr}. Don’t include any other tags.
• A <div> tag cannot be nested inside a <p> tag, and a <p> tag cannot be nested inside a <p> tag. A <div> tag can nest inside another <div>.
• <br> is one of the few tags that doesn’t have a closing tag, so documents can contain just <br>. Similarly for <hr>.
• Any attributes in an opening tag are ignored. For example, in <div class=”text”> the class attribute is ignored.
• The <title> tag is included in the head section between the head tags only, and not in the body section between the body tags.
• A DOCTYPE at the start of the document is not required.
• Assume there are no comments (<!-- -->) or entities (e.g., &lt; to represent <) in the HTML document.
• Assume that the ‘<’ and ‘>’ characters are used only as part of the HTML tags, and do not appear in the text within p, h1, h2, h3, or any other sections, or in attribute values.
• The input file name should be fixed as ‘file.html’. You do not need to add code to ask for a file name and input a file name. The program always reads from file.html.
