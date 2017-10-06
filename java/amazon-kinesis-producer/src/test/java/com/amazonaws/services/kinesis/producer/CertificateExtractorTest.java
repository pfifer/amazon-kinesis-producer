/*
 * Copyright 2017 Amazon.com, Inc. or its affiliates. All Rights Reserved. Licensed under the Amazon Software License
 * (the "License"). You may not use this file except in compliance with the License. A copy of the License is located at
 * http://aws.amazon.com/asl/ or in the "license" file accompanying this file. This file is distributed on an "AS IS"
 * BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 */
package com.amazonaws.services.kinesis.producer;

import static org.hamcrest.CoreMatchers.equalTo;
import static org.hamcrest.MatcherAssert.assertThat;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.nio.channels.FileChannel;
import java.nio.channels.FileLock;
import java.nio.file.Files;
import java.nio.file.StandardOpenOption;
import java.util.Arrays;
import java.util.List;

import org.apache.commons.io.Charsets;
import org.apache.commons.io.IOUtils;
import org.junit.Test;

public class CertificateExtractorTest {

    static final List<String> EXPECTED_CERTIFICATES = Arrays.asList("00673b5b.0", "0d69c7e1.0", "10531352.0",
            "111e6273.0", "1e8e7201.0", "219d9499.0", "23f4c490.0", "27af790d.0", "2add47b6.0", "343eb6cb.0",
            "399e7759.0", "3ad48a91.0", "524d9b43.0", "57692373.0", "5e4e69e7.0", "67495436.0", "69105f4f.0",
            "7999be0d.0", "7a7c655d.0", "7d453d8f.0", "81b9768f.0", "85cde254.0", "87753b0d.0", "8d6437c3.0",
            "9772ca32.0", "a2c66da8.0", "a7d2cf64.0", "b0f3e76e.0", "bc3f2570.0", "c491639e.0", "c7e2a638.0",
            "c90bc37d.0", "d41b5e2a.0", "e775ed2d.0", "ed39abd0.0", "facacbc6.0", "fb5fa911.0", "fd08c599.0");

    @Test
    public void testNoCertificatesExtraction() throws Exception {
        File tempDirectory = Files.createTempDirectory("kpl-ca-test").toFile();
        CertificateExtractor extractor = new CertificateExtractor();
        extractor.extractCertificates(tempDirectory);

        File caCertsDirectory = new File(tempDirectory, CertificateExtractor.CA_CERTS_DIRECTORY_NAME);
        verifyAllCertificates(caCertsDirectory);

    }

    @Test
    public void testDirectoryExistsOnExtractionWithMismatch() throws Exception {
        File tempDirectory = Files.createTempDirectory("kpl-ca-test").toFile();
        File caDirectory = new File(tempDirectory, CertificateExtractor.CA_CERTS_DIRECTORY_NAME);
        assertThat(caDirectory.mkdirs(), equalTo(true));
        File existingCert = new File(caDirectory, EXPECTED_CERTIFICATES.get(5));

        FileOutputStream fos = new FileOutputStream(existingCert);
        fos.write(new byte[] { 1, 2, 3, 4 });

        CertificateExtractor extractor = new CertificateExtractor();
        extractor.extractCertificates(tempDirectory);

        verifyAllCertificates(caDirectory);
    }

    private void verifyAllCertificates(File caCertsDirectory) throws IOException {
        ClassLoader classLoader = CertificateExtractor.class.getClassLoader();

        for (String expectedCert : EXPECTED_CERTIFICATES) {
            File resourceFile = new File(CertificateExtractor.CA_CERTS_DIRECTORY_NAME, expectedCert);

            InputStream is = classLoader.getResourceAsStream(resourceFile.getPath());
            List<String> expectedCertLines = IOUtils.readLines(is, Charsets.UTF_8);

            File actualCaFile = new File(caCertsDirectory, expectedCert);
            List<String> actualCertLines = Files.readAllLines(actualCaFile.toPath(), Charsets.UTF_8);

            assertThat(actualCertLines, equalTo(expectedCertLines));
        }
    }

    private static class LockHolder implements Runnable {

        final File lockFile;

        private LockHolder(File tempDirectory) {
            this.lockFile = new File(tempDirectory, "cacerts.lock");
        }

        @Override
        public void run() {
            holdLock();
        }

        private void holdLock() {
            try {
                try (FileLock lock = FileChannel.open(lockFile.toPath(), StandardOpenOption.CREATE, StandardOpenOption.WRITE).lock()) {
                    Thread.sleep(5000);
                }

            } catch (Exception ex) {
                throw new RuntimeException(ex);
            }
        }
    }

}