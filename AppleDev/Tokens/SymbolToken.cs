using System;
using System.Collections.Generic;
using System.Linq;
using System.Linq.Expressions;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media.Animation;
using System.Xml.Xsl;

namespace AppleDev.Tokens
{
   class SymbolToken : Token
   {
      private struct SymbolInfo
      {
         public SymbolInfo(string text, Symbol symbol)
         {
            this.text = text;
            this.symbol = symbol;
         }

         public string text;
         public Symbol symbol;
      }

      private static SymbolInfo[] symbols = new SymbolInfo[] {
         new SymbolInfo("(", Symbol.LeftParen),
         new SymbolInfo(")", Symbol.RightParen),
         new SymbolInfo("{", Symbol.LeftCurlyBracket),
         new SymbolInfo("}", Symbol.RightCurlyBracket),
         new SymbolInfo(";", Symbol.Semicolon)
         };

      public SymbolToken(Symbol symbol)
      {
         this.Symbol = symbol;
      }

      public Symbol Symbol
      {
         get;
         private set;
      }

      public static bool IsSymbolCharacter(char c)
      {
         return "(){};".Contains(c);
      }

      public static Token GetSymbolToken(ref string s)
      {
         foreach (var symbolInfo in symbols)
         {
            if (s.IndexOf(symbolInfo.text, StringComparison.InvariantCulture) == 0)
            {
               s = s.Substring(symbolInfo.text.Length);
               return new SymbolToken(symbolInfo.symbol);
            }
         }

         throw new CompileException("Invalid symbol character: " + s[0]);
      }
   }
}
