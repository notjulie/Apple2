using System;
using System.Collections.Generic;
using System.Linq;
using System.Linq.Expressions;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Xsl;

using Suna.Groups;

namespace Suna.Tokens
{
   /// <summary>
   /// Token extension specific to symbol tokens
   /// </summary>
   class SymbolToken : Token
   {
      #region Types / Constants

      private enum Error
      {
         UnexpectedSymbol
      }

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
         new SymbolInfo(";", Symbol.Semicolon),
         new SymbolInfo(",", Symbol.Comma),
         new SymbolInfo(".", Symbol.Dot)
         };

      #endregion

      #region Constructor

      public SymbolToken(TokenInfo info, Symbol symbol)
         :base(info)
      {
         this.Symbol = symbol;
      }

      #endregion

      #region Public Properties

      public Symbol Symbol
      {
         get;
         private set;
      }

      #endregion

      #region Public Methods

      public static bool IsSymbolCharacter(char c)
      {
         return "(){};,.".Contains(c);
      }

      public static Token GetSymbolToken(ref string s)
      {
         foreach (var symbolInfo in symbols)
         {
            if (s.StartsWith(symbolInfo.text, StringComparison.InvariantCulture))
            {
               s = s.Substring(symbolInfo.text.Length);
               return new SymbolToken(
                  new TokenInfo(symbolInfo.text),
                  symbolInfo.symbol
                  );
            }
         }

         throw new CompileException("Invalid symbol character: " + s[0]);
      }

      #endregion

      #region Base class overrides

      /// <summary>
      /// Reads a group that begins with this token
      /// </summary>
      /// <param name="enumerator"></param>
      /// <returns></returns>
      public override GroupItem ReadGroupItem(IEnumerator<Token> enumerator)
      {
         // we're currently pointing at our own position; move past it

         switch (Symbol)
         {
            case Symbol.LeftCurlyBracket:
               return CurlyBracketGroup.Read(enumerator);

            case Symbol.LeftParen:
               return ParentheticGroup.Read(enumerator);

            case Symbol.RightCurlyBracket:
            case Symbol.RightParen:
               throw new CompileException(Error.UnexpectedSymbol);

            default:
               return base.ReadGroupItem(enumerator);
         }
      }

      #endregion
   }
}
