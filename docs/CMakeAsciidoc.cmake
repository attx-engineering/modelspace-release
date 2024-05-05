#****************************************************
# Copyright ATTX 2023
#
# All rights reserved. This software, including all
# code, text, data, and documentation, is the
# proprietary and confidential information of ATTX.
#
# Any use, reproduction, or distribution of this
# software, in whole or in part, without the express
# written permission of ATTX is strictly prohibited.
#
# DISCLAIMERS:
# This software is provided "as is" without warranty
# of any kind, either expressed or implied, including,
# but not limited to, the implied warranties of
# merchantability and fitness for a particular purpose.
#
# ATTX assumes no responsibility
# for any errors or omissions in this software or
# documentation.
#
# IN NO EVENT SHALL ATTX BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
# GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
# OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
# ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
# STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
# OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
# THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
# OF SUCH DAMAGE.
#****************************************************
# A script to build via asciidoc
# First, identify all of our asciidoc targets
file(GLOB_RECURSE ASCIIDOC_TGT ${CMAKE_ARGV3}/*.adoc)

foreach(tgt ${ASCIIDOC_TGT})
    # Build our output doc name -- first strip our project dir
    string(REPLACE ${CMAKE_ARGV3}/ "" tgt_name ${tgt})

    # And actually make our document
    execute_process(COMMAND asciidoctor -D docs ${tgt})
endforeach()