// -----------------------------------------------------------------------------
// 'xdma' Register Definitions
// Revision: 6
// -----------------------------------------------------------------------------
// Generated on 2021-07-29 at 11:32 (UTC) by airhdl version 2021.06.1
// -----------------------------------------------------------------------------
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
// -----------------------------------------------------------------------------

#ifndef XDMA_REGS_H
#define XDMA_REGS_H

/* Revision number of the 'xdma' register map */
#define XDMA_REVISION 6

/* Default base address of the 'xdma' register map */
#define XDMA_DEFAULT_BASEADDR 0x43C60000

/* Register 'Address' */
#define ADDRESS_OFFSET 0x00000000 /* address offset of the 'Address' register */

/* Field  'Address.value' */
#define ADDRESS_VALUE_BIT_OFFSET 0 /* bit offset of the 'value' field */
#define ADDRESS_VALUE_BIT_WIDTH 32 /* bit width of the 'value' field */
#define ADDRESS_VALUE_BIT_MASK 0xFFFFFFFF /* bit mask of the 'value' field */
#define ADDRESS_VALUE_RESET 0x0 /* reset value of the 'value' field */

/* Register 'Length' */
#define LENGTH_OFFSET 0x00000004 /* address offset of the 'Length' register */

/* Field  'Length.value' */
#define LENGTH_VALUE_BIT_OFFSET 0 /* bit offset of the 'value' field */
#define LENGTH_VALUE_BIT_WIDTH 24 /* bit width of the 'value' field */
#define LENGTH_VALUE_BIT_MASK 0x00FFFFFF /* bit mask of the 'value' field */
#define LENGTH_VALUE_RESET 0x0 /* reset value of the 'value' field */

/* Register 'Command' */
#define COMMAND_OFFSET 0x00000008 /* address offset of the 'Command' register */

/* Field  'Command.Start' */
#define COMMAND_START_BIT_OFFSET 0 /* bit offset of the 'Start' field */
#define COMMAND_START_BIT_WIDTH 1 /* bit width of the 'Start' field */
#define COMMAND_START_BIT_MASK 0x00000001 /* bit mask of the 'Start' field */
#define COMMAND_START_RESET 0x0 /* reset value of the 'Start' field */

#endif  /* XDMA_REGS_H */
