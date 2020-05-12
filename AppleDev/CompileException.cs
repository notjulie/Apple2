using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.Text;
using System.Threading.Tasks;

namespace AppleDev
{
   /// <summary>
   /// Exception during compiling
   /// </summary>
   [Serializable]
   public class CompileException : Exception
   {
      /// <summary>
      /// Initializes a new instance of class CompileException
      /// </summary>
      public CompileException()
      {
      }

      /// <summary>
      /// Initializes a new instance of class CompileException
      /// </summary>
      /// <param name="message"></param>
      public CompileException(Enum message)
#pragma warning disable CA1062 // Validate arguments of public methods
         : this(message.ToString())
#pragma warning restore CA1062 // Validate arguments of public methods
      {
      }

      /// <summary>
      /// Initializes a new instance of class CompileException
      /// </summary>
      /// <param name="message"></param>
      public CompileException(string message)
         : base(message)
      {
      }

      /// <summary>
      /// Initializes a new instance of class CompileException
      /// </summary>
      /// <param name="message"></param>
      /// <param name="inner"></param>
      public CompileException(string message, Exception inner)
         : base(message, inner)
      {
      }

      /// <summary>
      /// Initializes a new instance of class CompileException
      /// </summary>
      /// <param name="message"></param>
      /// <param name="inner"></param>
      public CompileException(Enum message, Exception inner)
#pragma warning disable CA1062 // Validate arguments of public methods
         : this(message.ToString(), inner)
#pragma warning restore CA1062 // Validate arguments of public methods
      {
      }

      /// <summary>
      /// Initializes a new instance of class CompileException
      /// </summary>
      /// <param name="info"></param>
      /// <param name="context"></param>
      protected CompileException(SerializationInfo info, StreamingContext context)
         : base(info, context)
      {
      }
   }
}
